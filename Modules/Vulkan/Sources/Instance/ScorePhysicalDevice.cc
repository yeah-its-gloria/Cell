// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/BlockImpl.hh>
#include <Cell/Vulkan/Instance.hh>

namespace Cell::Vulkan {

uint16_t Instance::ScorePhysicalDevice(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(device, &properties);

    if (VK_API_VERSION_MAJOR(properties.apiVersion) < 1 || VK_API_VERSION_MINOR(properties.apiVersion) < 3) {
        return 0;
    }

    uint16_t score = 0;

    switch (properties.deviceType) {
    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: {
        score += 300;
    }

    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: {
        score += 200;
    }

    case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
    case VK_PHYSICAL_DEVICE_TYPE_CPU: {
        score += 100;
        break;
    }

    default: {
        break;
    }
    }

    if (properties.limits.maxPushConstantsSize >= 256) {
        score += 200;
    }

    // Feature checks

    VkPhysicalDeviceExtendedDynamicStateFeaturesEXT extendedDynamicStateFeature = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT,
        .pNext = nullptr,
        .extendedDynamicState = VK_FALSE
    };

    VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamicRenderingFeature = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR,
        .pNext = &extendedDynamicStateFeature,
        .dynamicRendering = VK_FALSE
    };

    VkPhysicalDeviceFeatures2 features = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
        .pNext = &dynamicRenderingFeature,
        .features = { }
    };

    vkGetPhysicalDeviceFeatures2(device, &features);

    if (!features.features.fullDrawIndexUint32 ||
        !features.features.independentBlend ||
        !features.features.geometryShader ||
        !features.features.tessellationShader ||
        !features.features.logicOp ||
        !features.features.depthClamp ||
        !features.features.fillModeNonSolid ||
        !features.features.wideLines ||
        !features.features.multiViewport ||
        !features.features.shaderFloat64 ||
        !features.features.shaderInt64 ||
        !features.features.shaderInt16 ||
        !dynamicRenderingFeature.dynamicRendering ||
        !extendedDynamicStateFeature.extendedDynamicState) {
        return 0;
    }

    // Queue family checks

    const uint64_t queues = Instance::QueryPhysicalDeviceQueues(device);
    uint32_t graphicsQueue = queues >> 32;
    uint32_t transferQueue = queues & 0xffffffff;

    if (transferQueue != (uint32_t)-1 && graphicsQueue != transferQueue) {
        score += 200;
    }

    return score;
}

}
