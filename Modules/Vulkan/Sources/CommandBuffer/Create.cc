// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/CommandBuffer.hh>

namespace Cell::Vulkan {

Wrapped<CommandBuffer*, Result> Device::CreateCommandBuffer(const QueueType type) {
    VkQueue& queueRef = this->deviceQueueGraphics;
    VkCommandPool& poolRef = this->graphicsPool;

    if (type == QueueType::Transfer && this->physicalDeviceQueueTransfer != (uint32_t)-1) {
        queueRef = this->deviceQueueTransfer;
        poolRef = this->transferPool;
    }

    const VkCommandBufferAllocateInfo bufferInfo = {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext              = nullptr,

        .commandPool        = poolRef,
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };

    VkCommandBuffer buffer = nullptr;
    const VkResult result = vkAllocateCommandBuffers(this->device, &bufferInfo, &buffer);
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

    return new CommandBuffer(buffer, queueRef, this);
}

}
