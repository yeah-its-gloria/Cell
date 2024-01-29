// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/Device.hh>

namespace Cell::Vulkan {

Wrapped<Device*, Result> Instance::CreateDevice() {
    const char* extensions[4] = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,

        VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
        VK_KHR_SHADER_NON_SEMANTIC_INFO_EXTENSION_NAME,

        VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME,
        // VK_EXT_SHADER_OBJECT_EXTENSION_NAME
    };

    return this->CreateDevice(extensions, 4);
}

Wrapped<Device*, Result> Instance::CreateDevice(const char** extensions, const uint32_t count, VkPhysicalDevice physicalDevice) {
    Wrapped<QueryPhysicalDeviceResult, Result> queryResult = this->QueryPhysicalDevice(physicalDevice);
    if (!queryResult.IsValid()) {
        return queryResult.Result();
    }

    QueryPhysicalDeviceResult physDev = queryResult.Unwrap();

    // Queues

    const float queuePriority = 1.f;

    const VkDeviceQueueCreateInfo queueCreateInfo[2] = {
        {
            .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext            = nullptr,
            .flags            = 0,

            .queueFamilyIndex = physDev.queues.graphics,
            .queueCount       = 1,
            .pQueuePriorities = &queuePriority
        },

        {
            .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext            = nullptr,
            .flags            = 0,

            .queueFamilyIndex = physDev.queues.transfer,
            .queueCount       = 1,
            .pQueuePriorities = &queuePriority
        }
    };

    // Features

    const VkPhysicalDeviceFeatures deviceFeatures = {
        .robustBufferAccess                      = VK_FALSE,
        .fullDrawIndexUint32                     = VK_TRUE,
        .imageCubeArray                          = VK_FALSE,
        .independentBlend                        = VK_TRUE,
        .geometryShader                          = VK_TRUE,
        .tessellationShader                      = VK_TRUE,
        .sampleRateShading                       = VK_FALSE,
        .dualSrcBlend                            = VK_FALSE,
        .logicOp                                 = VK_TRUE,
        .multiDrawIndirect                       = VK_FALSE,
        .drawIndirectFirstInstance               = VK_FALSE,
        .depthClamp                              = VK_TRUE,
        .depthBiasClamp                          = VK_FALSE,
        .fillModeNonSolid                        = VK_TRUE,
        .depthBounds                             = VK_FALSE,
        .wideLines                               = VK_TRUE,
        .largePoints                             = VK_FALSE,
        .alphaToOne                              = VK_FALSE,
        .multiViewport                           = VK_TRUE,
        .samplerAnisotropy                       = VK_TRUE,
        .textureCompressionETC2                  = VK_FALSE,
        .textureCompressionASTC_LDR              = VK_FALSE,
        .textureCompressionBC                    = VK_FALSE,
        .occlusionQueryPrecise                   = VK_FALSE,
        .pipelineStatisticsQuery                 = VK_FALSE,
        .vertexPipelineStoresAndAtomics          = VK_FALSE,
        .fragmentStoresAndAtomics                = VK_FALSE,
        .shaderTessellationAndGeometryPointSize  = VK_FALSE,
        .shaderImageGatherExtended               = VK_FALSE,
        .shaderStorageImageExtendedFormats       = VK_FALSE,
        .shaderStorageImageMultisample           = VK_FALSE,
        .shaderStorageImageReadWithoutFormat     = VK_FALSE,
        .shaderStorageImageWriteWithoutFormat    = VK_FALSE,
        .shaderUniformBufferArrayDynamicIndexing = VK_FALSE,
        .shaderSampledImageArrayDynamicIndexing  = VK_FALSE,
        .shaderStorageBufferArrayDynamicIndexing = VK_FALSE,
        .shaderStorageImageArrayDynamicIndexing  = VK_FALSE,
        .shaderClipDistance                      = VK_FALSE,
        .shaderCullDistance                      = VK_FALSE,
        .shaderFloat64                           = VK_TRUE,
        .shaderInt64                             = VK_TRUE,
        .shaderInt16                             = VK_TRUE,
        .shaderResourceResidency                 = VK_FALSE,
        .shaderResourceMinLod                    = VK_FALSE,
        .sparseBinding                           = VK_FALSE,
        .sparseResidencyBuffer                   = VK_FALSE,
        .sparseResidencyImage2D                  = VK_FALSE,
        .sparseResidencyImage3D                  = VK_FALSE,
        .sparseResidency2Samples                 = VK_FALSE,
        .sparseResidency4Samples                 = VK_FALSE,
        .sparseResidency8Samples                 = VK_FALSE,
        .sparseResidency16Samples                = VK_FALSE,
        .sparseResidencyAliased                  = VK_FALSE,
        .variableMultisampleRate                 = VK_FALSE,
        .inheritedQueries                        = VK_FALSE
    };

    // Device creation

    const VkPhysicalDeviceExtendedDynamicStateFeaturesEXT extendedDynamicStateFeature = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT,
        .pNext = nullptr,
        .extendedDynamicState = VK_TRUE
    };

    const VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamicRenderingFeature = {
        .sType            = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR,
        .pNext            = (void*)&extendedDynamicStateFeature,
        .dynamicRendering = VK_TRUE
    };

    const VkDeviceCreateInfo deviceInfo = {
        .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext                   = &dynamicRenderingFeature,
        .flags                   = 0,

        .queueCreateInfoCount    = (uint32_t)(physDev.queues.transfer != (uint32_t)-1 ? 2 : 1),
        .pQueueCreateInfos       = queueCreateInfo,

        .enabledLayerCount       = 0,
        .ppEnabledLayerNames     = nullptr,

        .enabledExtensionCount   = count,
        .ppEnabledExtensionNames = extensions,

        .pEnabledFeatures        = &deviceFeatures
    };

    VkDevice device = nullptr;
    const VkResult result = vkCreateDevice(physDev.device, &deviceInfo, nullptr, &device);
    switch (result) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        return Result::OutOfDeviceMemory;
    }

    case VK_ERROR_EXTENSION_NOT_PRESENT:
    case VK_ERROR_FEATURE_NOT_PRESENT: {
        return Result::FeatureMissing;
    }

    case VK_ERROR_DEVICE_LOST: {
        return Result::DeviceLost;
    }

    default: {
        System::Panic("vkCreateDevice failed");
    }
    }

    // Queue retrieval

    VkQueue graphics = nullptr;
    vkGetDeviceQueue(device, physDev.queues.graphics, 0, &graphics);

    VkQueue transfer = nullptr;
    if (physDev.queues.transfer != (uint32_t)-1) {
        vkGetDeviceQueue(device, physDev.queues.transfer, 0, &transfer);
    }

    return new Device(physDev.device,
                      physDev.queues.graphics,
                      physDev.queues.transfer,
                      physDev.properties,
                      device,
                      graphics,
                      transfer,
                      this->instance,
                      this->beginRendering,
                      this->endRendering,
                      this->setCullMode);
}

}
