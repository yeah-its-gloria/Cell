// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/CommandBufferManager.hh>

namespace Cell::Vulkan {

#define CHECK_RETURN if (result != Result::Success) { return result; }

Result CommandBufferManager::BeginCommands(const uint32_t index) {
    const VkCommandBufferBeginInfo beginInfo = {
        .sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext            = nullptr,
        .flags            = 0,

        .pInheritanceInfo = nullptr
    };

    const VkResult result = vkBeginCommandBuffer(this->buffers[index], &beginInfo);
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
        System::Panic("vkBeginCommandBuffer failed");
    }
    }

    return Result::Success;
}

Result CommandBufferManager::EndCommands(const uint32_t index) {
    const VkResult result = vkEndCommandBuffer(this->buffers[index]);
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
        System::Panic("vkEndCommandBuffer failed");
    }
    }

    return Result::Success;
}

Result CommandBufferManager::WriteCommandsSingle(const uint32_t index, const Command* commands, const uint32_t count) {
    Result result = this->BeginCommands(index);
    CHECK_RETURN

    result = this->WriteCommands(index, commands, count);
    CHECK_RETURN

    return this->EndCommands(index);
}


}
