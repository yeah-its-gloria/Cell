// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/CommandBuffer.hh>

namespace Cell::Vulkan {

Result CommandBuffer::Submit() {
    if (this->recordState != RecordState::Recorded) {
        return Result::InvalidState;
    }

    const VkFenceCreateInfo fenceInfo = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0
    };

    VkFence fence = nullptr;
    VkResult result = vkCreateFence(this->device->device, &fenceInfo, nullptr, &fence);
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
        System::Panic("vkCreateFence failed");
    }
    }

    const VkSubmitInfo submitInfo = {
        .sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext                = nullptr,

        .waitSemaphoreCount   = 0,
        .pWaitSemaphores      = nullptr,
        .pWaitDstStageMask    = nullptr,

        .commandBufferCount   = 1,
        .pCommandBuffers      = &this->buffer,

        .signalSemaphoreCount = 0,
        .pSignalSemaphores    = nullptr
    };

    result = vkQueueSubmit(this->queueRef, 1, &submitInfo, fence);
    switch (result) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        vkDestroyFence(this->device->device, fence, nullptr);
        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        vkDestroyFence(this->device->device, fence, nullptr);
        return Result::OutOfDeviceMemory;
    }

    case VK_ERROR_DEVICE_LOST: {
        vkDestroyFence(this->device->device, fence, nullptr);
        return Result::DeviceLost;
    }

    default: {
        System::Panic("vkQueueSubmit failed");
    }
    }

    result = vkWaitForFences(this->device->device, 1, &fence, VK_TRUE, UINT64_MAX);
    switch (result) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        vkDestroyFence(this->device->device, fence, nullptr);
        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        vkDestroyFence(this->device->device, fence, nullptr);
        return Result::OutOfDeviceMemory;
    }

    case VK_ERROR_DEVICE_LOST: {
        vkDestroyFence(this->device->device, fence, nullptr);
        return Result::DeviceLost;
    }

    default: {
        System::Panic("vkWaitForFences failed");
    }
    }

    vkDestroyFence(this->device->device, fence, nullptr);
    return Result::Success;
}

}
