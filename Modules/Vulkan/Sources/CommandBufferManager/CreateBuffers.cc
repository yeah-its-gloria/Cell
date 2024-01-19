// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/CommandBufferManager.hh>

namespace Cell::Vulkan {

Result CommandBufferManager::CreateBuffers(const uint32_t count) {
    const VkCommandBufferAllocateInfo bufferInfo = {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext              = nullptr,

        .commandPool        = this->pool,
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = count
    };

    this->buffers.SetCount(count);

    const VkResult result = vkAllocateCommandBuffers(this->instance->device, &bufferInfo, this->buffers.AsRaw());
    switch (result) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        return Result::OutOfDeviceMemory;
    }

    default: {
        System::Panic("vkAllocateCommandBuffers failed");
    }
    }

    return Result::Success;
}

}
