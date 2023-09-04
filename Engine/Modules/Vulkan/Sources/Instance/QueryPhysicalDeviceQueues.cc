// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/BlockImpl.hh>
#include <Cell/Vulkan/Instance.hh>

namespace Cell::Vulkan {

uint64_t Instance::QueryPhysicalDeviceQueues(VkPhysicalDevice device) {
    uint32_t graphicsQueue = (uint32_t)-1;
    uint32_t transferQueue = (uint32_t)-1;

    uint32_t queueFamilyPropertiesCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyPropertiesCount, nullptr);

    System::ManagedBlock <VkQueueFamilyProperties> queueFamilyProperties(queueFamilyPropertiesCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyPropertiesCount, queueFamilyProperties);

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

    return (((uint64_t)graphicsQueue) << 32) | transferQueue;
}

}
