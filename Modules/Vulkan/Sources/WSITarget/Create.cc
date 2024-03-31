// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Platform.hh"

namespace Cell::Vulkan {

Wrapped<WSITarget*, Result> Device::CreateWSITarget(Shell::IShell* shell, const uint8_t depth) {
    VkSurfaceKHR surface = nullptr;
    const Result platformResult = createPlatformSurface(&surface, this->instance->instance, shell);
    if (platformResult != Result::Success) {
        return platformResult;
    }

    Wrapped<SurfaceStaticInfo, Result> staticsResult = this->GetSurfaceStatics(surface);
    if (!staticsResult.IsValid()) {
        vkDestroySurfaceKHR(this->instance->instance, surface, nullptr);
        return staticsResult.Result();
    }

    Wrapped<SurfaceDynamicInfo, Result> dynamicsResult = this->GetSurfaceDynamics(surface, shell->GetDrawableExtent().Unwrap());
    if (!dynamicsResult.IsValid()) {
        vkDestroySurfaceKHR(this->instance->instance, surface, nullptr);
        return dynamicsResult.Result();
    }

    const SurfaceStaticInfo statics = staticsResult.Unwrap();
    const SurfaceDynamicInfo dynamics = dynamicsResult.Unwrap();

    const SwapchainInfo info = {
        .surface = surface,
        .format = statics.format,
        .mode = statics.mode,
        .capabilities = dynamics.capabilities,
        .extent = dynamics.extent,
        .depth = depth
    };

    Wrapped<SwapchainData, Result> swapchainResult = this->CreateSwapchainForSurface(info);
    if (!swapchainResult.IsValid()) {
        vkDestroySurfaceKHR(this->instance->instance, surface, nullptr);
        return swapchainResult.Result();
    }

    SwapchainData swapchain = swapchainResult.Unwrap();

    Wrapped<Image*, Result> depthResult = this->CreateDepthStencilImage(dynamics.extent);
    if (!depthResult.IsValid()) {
        vkDestroySwapchainKHR(this->device, swapchain.swapchain, nullptr);
        vkDestroySurfaceKHR(this->instance->instance, surface, nullptr);
        return depthResult.Result();
    }

    Image* depthImage = depthResult.Unwrap();

    // sync

    const VkSemaphoreCreateInfo semaphoreInfo = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0
    };

    const VkFenceCreateInfo fenceInfo = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT
    };

    Collection::List<VkSemaphore> imageAvailable(depth);
    Collection::List<VkSemaphore> renderFinished(depth);
    Collection::List<VkFence> inFlightFrames(depth);

    VkResult result = VK_ERROR_UNKNOWN;
    for (size_t i = 0; i < depth; i++) {
        result = vkCreateSemaphore(this->device, &semaphoreInfo, nullptr, &imageAvailable[i]);
        if (result != VK_SUCCESS) {
            break;
        }

        result = vkCreateSemaphore(this->device, &semaphoreInfo, nullptr, &renderFinished[i]);
        if (result != VK_SUCCESS) {
            break;
        }

        result = vkCreateFence(this->device, &fenceInfo, nullptr, &inFlightFrames[i]);
        if (result != VK_SUCCESS) {
            break;
        }
    }

    switch (result) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        for (VkSemaphore& semaphore : imageAvailable) {
            if (semaphore != nullptr) {
                vkDestroySemaphore(this->device, semaphore, nullptr);
            }
        }

        for (VkSemaphore& semaphore : renderFinished) {
            if (semaphore != nullptr) {
                vkDestroySemaphore(this->device, semaphore, nullptr);
            }
        }

        for (VkFence& fence : inFlightFrames) {
            if (fence != nullptr) {
                vkDestroyFence(this->device, fence, nullptr);
            }
        }

        delete depthImage;

        for (VkImageView& view : swapchain.views) {
            vkDestroyImageView(this->device, view, nullptr);
        }

        vkDestroySwapchainKHR(this->device, swapchain.swapchain, nullptr);
        vkDestroySurfaceKHR(this->instance->instance, surface, nullptr);
        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        for (VkSemaphore& semaphore : imageAvailable) {
            if (semaphore != nullptr) {
                vkDestroySemaphore(this->device, semaphore, nullptr);
            }
        }

        for (VkSemaphore& semaphore : renderFinished) {
            if (semaphore != nullptr) {
                vkDestroySemaphore(this->device, semaphore, nullptr);
            }
        }

        for (VkFence& fence : inFlightFrames) {
            if (fence != nullptr) {
                vkDestroyFence(this->device, fence, nullptr);
            }
        }

        delete depthImage;

        for (VkImageView& view : swapchain.views) {
            vkDestroyImageView(this->device, view, nullptr);
        }

        vkDestroySwapchainKHR(this->device, swapchain.swapchain, nullptr);
        vkDestroySurfaceKHR(this->instance->instance, surface, nullptr);
        return Result::OutOfDeviceMemory;
    }

    default: {
        System::Panic("vkCreateSemaphore/vkCreateFence failed");
    }
    }

    return new WSITarget(surface, depth, dynamics.capabilities, statics.format, statics.mode,
                         dynamics.extent, swapchain.swapchain, swapchain.images, swapchain.views,
                         depthImage, imageAvailable, renderFinished, inFlightFrames,
                         this, shell);
}

}
