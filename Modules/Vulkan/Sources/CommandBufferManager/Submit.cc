// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/CommandBufferManager.hh>

namespace Cell::Vulkan {

Result CommandBufferManager::Submit(const uint32_t index) {
    if (this->buffers.GetCount() < index) {
        return Result::InvalidParameters;
    }

    const VkSubmitInfo submitInfo = {
        .sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext                = nullptr,

        .waitSemaphoreCount   = 0,
        .pWaitSemaphores      = nullptr,
        .pWaitDstStageMask    = nullptr,

        .commandBufferCount   = 1,
        .pCommandBuffers      = &this->buffers[index],

        .signalSemaphoreCount = 0,
        .pSignalSemaphores    = nullptr
    };

    VkResult result = vkQueueSubmit(this->queue, 1, &submitInfo, VK_NULL_HANDLE);
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

    case VK_ERROR_DEVICE_LOST: {
        return Result::DeviceLost;
    }

    default: {
        System::Panic("vkQueueSubmit failed");
    }
    }

    result = vkQueueWaitIdle(this->queue);
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

    case VK_ERROR_DEVICE_LOST: {
        return Result::DeviceLost;
    }

    default: {
        System::Panic("vkQueueWaitIdle failed");
    }
    }

    return Result::Success;
}

}
