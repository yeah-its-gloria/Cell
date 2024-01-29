// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/OpenXR/VulkanTarget.hh>

namespace Cell::OpenXR {

Result VulkanTarget::CreateSwapchains() {
    XrSwapchainCreateInfo swapchainInfo = {
        .type        = XR_TYPE_SWAPCHAIN_CREATE_INFO,
        .next        = nullptr,

        .createFlags = XR_SWAPCHAIN_CREATE_PROTECTED_CONTENT_BIT,
        .usageFlags  = XR_SWAPCHAIN_USAGE_SAMPLED_BIT | XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT,

        .format      = VK_FORMAT_R8G8B8A8_SRGB,

        .sampleCount = this->viewConfig->recommendedSwapchainSampleCount,
        .width       = this->viewConfig->recommendedImageRectWidth,
        .height      = this->viewConfig->recommendedImageRectHeight,

        .faceCount   = 1,
        .arraySize   = 1,
        .mipCount    = 1
    };

    XrResult result = xrCreateSwapchain(this->instance->session, &swapchainInfo, &this->colorSwapchain);
    switch (result) {
    case XR_SUCCESS: {
        break;
    }

    default: {
        System::Panic("xrCreateSwapchain failed");
    }
    }

    swapchainInfo.usageFlags = XR_SWAPCHAIN_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    swapchainInfo.format     = VK_FORMAT_D32_SFLOAT;

    result = xrCreateSwapchain(this->instance->session, &swapchainInfo, &this->depthSwapchain);
    switch (result) {
    case XR_SUCCESS: {
        break;
    }

    default: {
        System::Panic("xrCreateSwapchain failed");
    }
    }

    uint32_t count = 0;
    result = xrEnumerateSwapchainImages(this->colorSwapchain, 0, &count, nullptr);
    switch (result) {
    case XR_SUCCESS: {
        break;
    }

    default: {
        System::Panic("xrEnumerateSwapchainImages failed");
    }
    }

    uint32_t depthCount = 0;
    result = xrEnumerateSwapchainImages(this->depthSwapchain, 0, &depthCount, nullptr);
    switch (result) {
    case XR_SUCCESS: {
        break;
    }

    default: {
        System::Panic("xrEnumerateSwapchainImages failed");
    }
    }

    CELL_ASSERT(count == depthCount);

    this->swapchainColorImages.SetCount(count);
    this->swapchainDepthImages.SetCount(count);

    this->swapchainColorImageViews.SetCount(count);
    this->swapchainDepthImageViews.SetCount(count);

    for (XrSwapchainImageVulkanKHR image : this->swapchainColorImages) {
        image.type = XR_TYPE_SWAPCHAIN_IMAGE_VULKAN_KHR;
    }

    for (XrSwapchainImageVulkanKHR image : this->swapchainDepthImages) {
        image.type = XR_TYPE_SWAPCHAIN_IMAGE_VULKAN_KHR;
    }

    result = xrEnumerateSwapchainImages(this->colorSwapchain, count, &count, (XrSwapchainImageBaseHeader*)&this->swapchainColorImages);
    switch (result) {
    case XR_SUCCESS: {
        break;
    }

    default: {
        System::Panic("xrEnumerateSwapchainImages failed");
    }
    }

    result = xrEnumerateSwapchainImages(this->depthSwapchain, count, &count, (XrSwapchainImageBaseHeader*)&this->swapchainDepthImages);
    switch (result) {
    case XR_SUCCESS: {
        break;
    }

    default: {
        System::Panic("xrEnumerateSwapchainImages failed");
    }
    }

    for (uint8_t i= 0; i < this->swapchainColorImages.GetCount(); i++) {
        Vulkan::Result vkResult = this->instance->vulkanDevice->CreateImageView(this->swapchainColorImageViews[i], this->swapchainColorImages[i].image, VK_FORMAT_R8G8B8A8_SRGB);
        switch (vkResult) {
        case Vulkan::Result::Success: {
            break;
        }

        case Vulkan::Result::OutOfHostMemory:
        case Vulkan::Result::OutOfDeviceMemory: {
            return Result::OutOfMemory;
        }

        default: {
            System::Panic("Cell::Vulkan::Device::CreateImageView failed");
        }
        }

        vkResult = this->instance->vulkanDevice->CreateImageView(this->swapchainDepthImageViews[i], this->swapchainDepthImages[i].image, VK_FORMAT_D32_SFLOAT, VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_DEPTH_BIT);
        switch (vkResult) {
        case Vulkan::Result::Success: {
            break;
        }

        case Vulkan::Result::OutOfHostMemory:
        case Vulkan::Result::OutOfDeviceMemory: {
            return Result::OutOfMemory;
        }

        default: {
            System::Panic("Cell::Vulkan::Device::CreateImageView failed");
        }
        }
    }

    this->projectionView.subImage.swapchain               = this->colorSwapchain;
    this->projectionView.subImage.imageRect.extent.width  = this->viewConfig->recommendedImageRectWidth;
    this->projectionView.subImage.imageRect.extent.height = this->viewConfig->recommendedImageRectHeight;

    this->depthView.subImage.swapchain                    = this->depthSwapchain;
    this->depthView.subImage.imageRect.extent.width       = this->viewConfig->recommendedImageRectWidth;
    this->depthView.subImage.imageRect.extent.height      = this->viewConfig->recommendedImageRectHeight;

    return Result::Success;
}

}
