// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Renderer/Vulkan/Instance.hh>

namespace Cell::Renderer::Vulkan {

uint16_t Instance::ScorePhysicalDevice(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(device, &properties);

    if (VK_API_VERSION_MAJOR(properties.apiVersion) < 1 || VK_API_VERSION_MINOR(properties.apiVersion) < 2) {
        return 0;
    }

    uint16_t score = 0;

    switch (properties.deviceType) {
    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: {
        score += 300;
        break;
    }

    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: {
        score += 200;
        break;
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

    VkPhysicalDeviceScalarBlockLayoutFeatures scalarBlockLayoutFeature = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES,
        .pNext = nullptr,
        .scalarBlockLayout = VK_FALSE
    };

    VkPhysicalDeviceExtendedDynamicStateFeaturesEXT extendedDynamicStateFeature = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT,
        .pNext = &scalarBlockLayoutFeature,
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

    // TODO: check which features we actually want lol
    // BUG: yolo-ing this for macOS

    /*if (features.features.fullDrawIndexUint32            == VK_FALSE ||
        features.features.independentBlend               == VK_FALSE ||
        features.features.geometryShader                 == VK_FALSE ||
        features.features.tessellationShader             == VK_FALSE ||
        features.features.logicOp                        == VK_FALSE ||
        features.features.depthClamp                     == VK_FALSE ||
        features.features.fillModeNonSolid               == VK_FALSE ||
        features.features.wideLines                      == VK_FALSE ||
        features.features.multiViewport                  == VK_FALSE ||
        features.features.shaderFloat64                  == VK_FALSE ||
        features.features.shaderInt64                    == VK_FALSE ||
        features.features.shaderInt16                    == VK_FALSE ||
        dynamicRenderingFeature.dynamicRendering         == VK_FALSE ||
        extendedDynamicStateFeature.extendedDynamicState == VK_FALSE ||
        scalarBlockLayoutFeature.scalarBlockLayout       == VK_FALSE
    ) {
        return 0;
    }*/

    // Queue family checks

    const Instance::PhysicalDeviceQueues queues = Instance::QueryPhysicalDeviceQueues(device);
    if (queues.transfer != (uint32_t)-1) {
        score += 200;
    }

    return score;
}

}
