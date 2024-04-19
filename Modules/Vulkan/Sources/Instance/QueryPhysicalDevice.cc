// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Collection/List.hh>
#include <Cell/Vulkan/Instance.hh>

namespace Cell::Vulkan {

Wrapped<Instance::QueryPhysicalDeviceResult, Result> Instance::QueryPhysicalDevice(VkPhysicalDevice CELL_NULLABLE device) {
    if (device == nullptr) {
        uint32_t deviceCount = 0;
        VkResult vk_result = vkEnumeratePhysicalDevices(this->instance, &deviceCount, nullptr);
        switch (vk_result) {
        case VK_SUCCESS: {
            break;
        }

        case VK_ERROR_OUT_OF_HOST_MEMORY: {
            return Result::OutOfHostMemory;
        }

        case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
            return Result::OutOfDeviceMemory;
        }

        default: {
            System::Panic("vkEnumeratePhysicalDevices failed");
        }
        }

        Collection::List<VkPhysicalDevice> devices(deviceCount);
        vk_result = vkEnumeratePhysicalDevices(this->instance, &deviceCount, devices.AsRaw());
        switch (vk_result) {
        case VK_SUCCESS: {
            break;
        }

        case VK_ERROR_OUT_OF_HOST_MEMORY: {
            return Result::OutOfHostMemory;
        }

        case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
            return Result::OutOfDeviceMemory;
        }

        default: {
            System::Panic("vkEnumeratePhysicalDevices failed");
        }
        }

        uint16_t score = 0;
        for (VkPhysicalDevice& dev : devices) {
            const uint16_t temp_score = Instance::ScorePhysicalDevice(dev);
            if (temp_score > score) {
                score = temp_score;
                device = dev;
            }
        }

        if (device == nullptr) {
            return Result::RunningOnIncompatibleHardware;
        }
    }

    const Instance::PhysicalDeviceQueues queues = Instance::QueryPhysicalDeviceQueues(device);

#pragma clang diagnostic ignored "-Wmissing-field-initializers"
    Instance::QueryPhysicalDeviceResult result = {
        .device = device,
        .queues = queues
    };

    vkGetPhysicalDeviceMemoryProperties(result.device, &result.properties);
    return result;
}

Instance::PhysicalDeviceQueues Instance::QueryPhysicalDeviceQueues(VkPhysicalDevice device) {
    uint32_t graphicsQueue = (uint32_t)-1;
    uint32_t transferQueue = (uint32_t)-1;

    uint32_t queueFamilyPropertiesCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyPropertiesCount, nullptr);

    Collection::List<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyPropertiesCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyPropertiesCount, queueFamilyProperties.AsRaw());

    for (uint32_t index = 0; index < queueFamilyPropertiesCount; index++) {
        if (graphicsQueue == (uint32_t)-1 && queueFamilyProperties[index].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphicsQueue = index;
        }

        if (transferQueue == (uint32_t)-1 && queueFamilyProperties[index].queueFlags & VK_QUEUE_TRANSFER_BIT && index != graphicsQueue) {
            transferQueue = index;
        }

        if (graphicsQueue != (uint32_t)-1 && transferQueue != (uint32_t)-1) {
            break;
        }
    }

    CELL_ASSERT(graphicsQueue != (uint32_t)-1);

    return Instance::PhysicalDeviceQueues { .graphics = graphicsQueue, .transfer = transferQueue };
}

}
