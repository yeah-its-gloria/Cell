// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/WSITarget.hh>

namespace Cell::Vulkan {

Result WSITarget::CreateSwapchain() {
    if (this->surface == nullptr) {
        return Result::InvalidState;
    }

    // Swapchain creation

    const VkSwapchainCreateInfoKHR swapchainInfo = {
        .sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext                 = nullptr,
        .flags                 = 0,

        .surface               = this->surface,

        .minImageCount         = this->swapchainDepth,

        .imageFormat           = this->format.format,
        .imageColorSpace       = this->format.colorSpace,
        .imageExtent           = this->extent,
        .imageArrayLayers      = 1,
        .imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE,

        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices   = nullptr,

        .preTransform          = this->capabilities.currentTransform,

        .compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,

        .presentMode           = this->presentMode,

        .clipped               = VK_TRUE,

        .oldSwapchain          = nullptr
    };

    VkResult vkResult = vkCreateSwapchainKHR(this->instance->device, &swapchainInfo, nullptr, &this->swapchain);
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

    case VK_ERROR_SURFACE_LOST_KHR: {
        return Result::SurfaceLost;
    }

    default: {
        System::Panic("vkCreateSwapchainKHR failed");
    }
    }

    // Image retrieval

    uint32_t imageCount = 0;
    vkResult = vkGetSwapchainImagesKHR(this->instance->device, this->swapchain, &imageCount, nullptr);
    switch (vkResult) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        vkDestroySwapchainKHR(this->instance->device, this->swapchain, nullptr);
        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        vkDestroySwapchainKHR(this->instance->device, this->swapchain, nullptr);
        return Result::OutOfDeviceMemory;
    }

    default: {
        System::Panic("vkGetSwapchainImagesKHR failed");
    }
    }

    this->swapchainImages.SetCount(imageCount);

    vkResult = vkGetSwapchainImagesKHR(this->instance->device, this->swapchain, &imageCount, &this->swapchainImages);
    switch (vkResult) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        this->swapchainImages.Reset();
        vkDestroySwapchainKHR(this->instance->device, this->swapchain, nullptr);
        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        this->swapchainImages.Reset();
        vkDestroySwapchainKHR(this->instance->device, this->swapchain, nullptr);
        return Result::OutOfDeviceMemory;
    }

    default: {
        System::Panic("vkGetSwapchainImagesKHR failed");
    }
    }

    // Image view creation

    this->swapchainImageViews.SetCount(imageCount);

    Result result = Result::Success;
    for (uint32_t index = 0; index < imageCount; index++) {
        result = this->instance->CreateImageView(this->swapchainImageViews[index], this->swapchainImages[index], this->format.format);
        if (result != Result::Success) {
            break;
        }
    }

    if (result != Result::Success) {
        this->swapchainImageViews.Reset();
        this->swapchainImages.Reset();

        vkDestroySwapchainKHR(this->instance->device, this->swapchain, nullptr);
        return result;
    }

    return Result::Success;
}

}
