// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/Device.hh>

namespace Cell::Vulkan {

uint32_t Device::GetMemoryTypeIndex(VkBuffer buffer, const VkMemoryPropertyFlags type) {
    VkMemoryRequirements requirements;
    vkGetBufferMemoryRequirements(this->device, buffer, &requirements);

    uint32_t typeIndex = (uint32_t)-1;
    for (uint32_t i = 0; i < this->physicalDeviceProperties.memoryTypeCount; i++) {
        if (requirements.memoryTypeBits & (1 << i) && this->physicalDeviceProperties.memoryTypes[i].propertyFlags & type) {
            typeIndex = i;
            break;
        }
    }

    CELL_ASSERT(typeIndex != (uint32_t)-1);

    return typeIndex;
}

uint32_t Device::GetMemoryTypeIndex(VkImage image, const VkMemoryPropertyFlags type) {
    VkMemoryRequirements requirements;
    vkGetImageMemoryRequirements(this->device, image, &requirements);

    uint32_t typeIndex = (uint32_t)-1;
    for (uint32_t i = 0; i < this->physicalDeviceProperties.memoryTypeCount; i++) {
        if (requirements.memoryTypeBits & (1 << i) && this->physicalDeviceProperties.memoryTypes[i].propertyFlags & type) {
            typeIndex = i;
            break;
        }
    }

    CELL_ASSERT(typeIndex != (uint32_t)-1);

    return typeIndex;
}


}
