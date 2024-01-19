// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/CommandBufferManager.hh>

namespace Cell::Vulkan {

Wrapped<CommandBufferManager*, Result> Instance::CreateCommandBufferManager(const QueueType type, const bool resetIndividually) {
    if (this->device == nullptr) {
        return Result::InvalidState;
    }

    uint32_t index = this->physicalDeviceQueueGraphics;
    VkQueue queue = this->deviceQueueGraphics;

    if (type == QueueType::Transfer && this->physicalDeviceQueueTransfer != (uint32_t)-1) {
        index = this->physicalDeviceQueueTransfer;
        queue = this->deviceQueueGraphics;
    }

    const VkCommandPoolCreateInfo poolInfo = {
        .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext            = nullptr,
        .flags            = resetIndividually ? VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT : (VkCommandPoolCreateFlags)0,

        .queueFamilyIndex = index,
    };

    VkCommandPool pool = nullptr;
    const VkResult result = vkCreateCommandPool(this->device, &poolInfo, nullptr, &pool);
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

    default: {
        System::Panic("vkCreateCommandPool failed");
    }
    }

    return new CommandBufferManager(this, pool, queue);
}

}
