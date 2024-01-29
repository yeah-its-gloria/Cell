// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/WSITarget.hh>

namespace Cell::Vulkan {

#define CHECK_RETURN if (result != Result::Success) { return result; }

Result WSITarget::Recreate() {
    if (this->swapchain == nullptr) {
        return Result::InvalidState;
    }

    VkResult vkResult = vkDeviceWaitIdle(this->device->device);
    switch (vkResult) {
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
        System::Panic("vkDeviceWaitIdle failed");
    }
    }

    if (this->swapchainImageViews.GetCount() > 0) {
        for (VkImageView imageView : this->swapchainImageViews) {
            vkDestroyImageView(this->device->device, imageView, nullptr);
        }
    }

    vkDestroySwapchainKHR(this->device->device, this->swapchain, nullptr);

    Result result = this->RetrieveProperties();
    if (result != Result::Success) {
        return result;
    }

    result = this->CreateSwapchain();
    if (result != Result::Success) {
        return result;
    }

    return this->PrepareSwapchain();
}

}
