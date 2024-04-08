// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/Collection/Single.hh>
#include <Cell/Vulkan/CommandBuffer.hh>

namespace Cell::Vulkan {

Wrapped<Device::SwapchainData, Result> Device::CreateSwapchainForSurface(const SwapchainInfo& info) {
    const VkSwapchainCreateInfoKHR swapchainInfo = {
        .sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext                 = nullptr,
        .flags                 = 0,

        .surface               = info.surface,

        .minImageCount         = info.depth,

        .imageFormat           = info.format.format,
        .imageColorSpace       = info.format.colorSpace,
        .imageExtent           = info.extent,
        .imageArrayLayers      = 1,
        .imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE,

        .queueFamilyIndexCount = 1,
        .pQueueFamilyIndices   = &this->physicalDeviceQueueGraphics,

        .preTransform          = info.capabilities.currentTransform,

        .compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,

        .presentMode           = info.mode,

        .clipped               = VK_TRUE,

        .oldSwapchain          = nullptr
    };

    VkSwapchainKHR swapchain = nullptr;
    VkResult result = vkCreateSwapchainKHR(this->device, &swapchainInfo, nullptr, &swapchain);
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
    result = vkGetSwapchainImagesKHR(this->device, swapchain, &imageCount, nullptr);
    switch (result) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        vkDestroySwapchainKHR(this->device, swapchain, nullptr);
        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        vkDestroySwapchainKHR(this->device, swapchain, nullptr);
        return Result::OutOfDeviceMemory;
    }

    default: {
        System::Panic("vkGetSwapchainImagesKHR failed");
    }
    }

    Collection::List<VkImage> images(imageCount);
    result = vkGetSwapchainImagesKHR(this->device, swapchain, &imageCount, images.AsRaw());
    switch (result) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        vkDestroySwapchainKHR(this->device, swapchain, nullptr);
        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        vkDestroySwapchainKHR(this->device, swapchain, nullptr);
        return Result::OutOfDeviceMemory;
    }

    default: {
        System::Panic("vkGetSwapchainImagesKHR failed");
    }
    }

    // Image view creation

    Collection::List<VkImageView> views(imageCount);

    for (uint32_t index = 0; index < imageCount; index++) {
        Wrapped<VkImageView, Result> viewResult = this->CreateImageView(images[index], info.format.format);
        if (!viewResult.IsValid()) {
            vkDestroySwapchainKHR(this->device, swapchain, nullptr);
            return viewResult.Result();
        }

        views[index] = viewResult.Unwrap();
    }

    // Image render prep

    Wrapped<CommandBuffer*, Result> commandBufferResult = this->CreateCommandBuffer();
    if (!commandBufferResult.IsValid()) {
        return commandBufferResult.Result();
    }

    ScopedObject<CommandBuffer> commandBuffer = commandBufferResult.Unwrap();

    Result cellResult = commandBuffer->Begin();
    if (cellResult != Result::Success) {
        return cellResult;
    }

    for (VkImage& image : images) {
        const VkImageMemoryBarrier barrierInfo = {
            .sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext                           = nullptr,
            .srcAccessMask                   = VK_ACCESS_NONE,
            .dstAccessMask                   = VK_ACCESS_NONE,
            .oldLayout                       = VK_IMAGE_LAYOUT_UNDEFINED,
            .newLayout                       = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            .srcQueueFamilyIndex             = 0,
            .dstQueueFamilyIndex             = 0,
            .image                           = image,
            .subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
            .subresourceRange.baseMipLevel   = 0,
            .subresourceRange.levelCount     = 1,
            .subresourceRange.baseArrayLayer = 0,
            .subresourceRange.layerCount     = 1
        };

        const CommandParameters::InsertBarrier barrierParams = {
            .stageMaskSource      = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .stageMaskDestination = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
            .dependencyFlags      = 0,
            .barrierCount         = 0,
            .barriers             = nullptr,
            .bufferCount          = 0,
            .buffers              = nullptr,
            .imageCount           = 1,
            .images               = &barrierInfo
        };

        const Command command = { CommandType::InsertBarrier, &barrierParams };
        cellResult = commandBuffer->Write(Collection::Single(command));
        if (cellResult != Result::Success) {
            return cellResult;
        }
    }

    cellResult = commandBuffer->End();
    if (cellResult != Result::Success) {
        return cellResult;
    }

    cellResult = commandBuffer->Submit();
    if (cellResult != Result::Success) {
        return cellResult;
    }

    return SwapchainData { swapchain, images, views };
}

}
