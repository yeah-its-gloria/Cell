// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/WSITarget.hh>

namespace Cell::Vulkan {

Result WSITarget::CreateSwapchain() {
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

        .queueFamilyIndexCount = 1,
        .pQueueFamilyIndices   = &this->device->physicalDeviceQueueGraphics,

        .preTransform          = this->capabilities.currentTransform,

        .compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,

        .presentMode           = this->presentMode,

        .clipped               = VK_TRUE,

        .oldSwapchain          = nullptr
    };

    VkResult result = vkCreateSwapchainKHR(this->device->device, &swapchainInfo, nullptr, &this->swapchain);
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

    case VK_ERROR_SURFACE_LOST_KHR: {
        return Result::SurfaceLost;
    }

    default: {
        System::Panic("vkCreateSwapchainKHR failed");
    }
    }

    // Image retrieval

    uint32_t imageCount = 0;
    result = vkGetSwapchainImagesKHR(this->device->device, this->swapchain, &imageCount, nullptr);
    switch (result) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        vkDestroySwapchainKHR(this->device->device, this->swapchain, nullptr);
        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        vkDestroySwapchainKHR(this->device->device, this->swapchain, nullptr);
        return Result::OutOfDeviceMemory;
    }

    default: {
        System::Panic("vkGetSwapchainImagesKHR failed");
    }
    }

    this->swapchainImages.SetCount(imageCount);

    result = vkGetSwapchainImagesKHR(this->device->device, this->swapchain, &imageCount, this->swapchainImages.AsRaw());
    switch (result) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        this->swapchainImages.Reset();
        vkDestroySwapchainKHR(this->device->device, this->swapchain, nullptr);
        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        this->swapchainImages.Reset();
        vkDestroySwapchainKHR(this->device->device, this->swapchain, nullptr);
        return Result::OutOfDeviceMemory;
    }

    default: {
        System::Panic("vkGetSwapchainImagesKHR failed");
    }
    }

    // Image view creation

    this->swapchainImageViews.SetCount(imageCount);

    Result cellResult = Result::Success;
    for (uint32_t index = 0; index < imageCount; index++) {
        cellResult = this->device->CreateImageView(this->swapchainImageViews[index], this->swapchainImages[index], this->format.format);
        if (cellResult != Result::Success) {
            break;
        }
    }

    if (cellResult != Result::Success) {
        this->swapchainImageViews.Reset();
        this->swapchainImages.Reset();

        vkDestroySwapchainKHR(this->device->device, this->swapchain, nullptr);
        return cellResult;
    }

    return Result::Success;
}

}
