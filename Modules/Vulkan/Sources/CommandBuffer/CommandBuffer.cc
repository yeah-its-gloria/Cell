// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/CommandBuffer.hh>

namespace Cell::Vulkan {

Result CommandBuffer::Begin() {
    if (this->recordState == RecordState::Recording) {
        return Result::InvalidState;
    }

    const VkCommandBufferBeginInfo beginInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = nullptr,
        .flags = 0,

        .pInheritanceInfo = nullptr
    };

    const VkResult result = vkBeginCommandBuffer(this->buffer, &beginInfo);
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

    this->recordState = RecordState::Recording;
    return Result::Success;
}

Result CommandBuffer::End() {
    if (this->recordState != RecordState::Recording) {
        return Result::InvalidState;
    }

    const VkResult result = vkEndCommandBuffer(this->buffer);
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

    this->recordState = RecordState::Recorded;
    return Result::Success;
}

Result CommandBuffer::Reset() {
    if (this->recordState != RecordState::Recorded) {
        return Result::InvalidState;
    }

    const VkResult result = vkResetCommandBuffer(this->buffer, 0);
    switch (result) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        return Result::OutOfDeviceMemory;
    }

    default: {
        System::Panic("vkResetCommandBuffer failed");
    }
    }

    this->recordState = RecordState::Initialized;
    return Result::Success;
}

Result CommandBuffer::WriteSinglePass(const Collection::IEnumerable<const Command>& commands) {
    switch (recordState) {
    case RecordState::Initialized: {
        break;
    }

    case RecordState::Recording: {
        return Result::InvalidState;
    }

    case RecordState::Recorded: {
        const Result result = this->Reset();
        if (result != Result::Success) {
            return result;
        }

        break;
    }
    }

    Result result = this->Begin();
    if (result != Result::Success) {
        return result;
    }

    result = this->Write(commands);
    if (result != Result::Success) {
        return result;
    }

    return this->End();
}

}
