// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Renderer/Vulkan/WSITarget.hh>

namespace Cell::Renderer::Vulkan {

Result WSITarget::Recreate() {
    const VkResult vkResult = vkDeviceWaitIdle(this->device->device);
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

    // Fetch new data

    Wrapped<Device::SurfaceDynamicInfo, Result> dynamicsResult = this->device->GetSurfaceDynamics(this->surface, this->shell->GetDrawableExtent().Unwrap());
    if (!dynamicsResult.IsValid()) {
        return dynamicsResult.Result();
    }

    const Device::SurfaceDynamicInfo dynamics = dynamicsResult.Unwrap();

    const Device::SwapchainInfo info = {
        .surface = this->surface,
        .format = this->format,
        .mode = this->mode,
        .capabilities = dynamics.capabilities,
        .extent = dynamics.extent,
        .depth = depth
    };

    Wrapped<Device::SwapchainData, Result> swapchainResult = this->device->CreateSwapchainForSurface(info);
    if (!swapchainResult.IsValid()) {
        return swapchainResult.Result();
    }

    Wrapped<Image*, Result> depthResult = this->device->CreateDepthStencilImage(dynamics.extent);
    if (!depthResult.IsValid()) {
        vkDestroySwapchainKHR(this->device->device, swapchainResult.Unwrap().swapchain, nullptr);
        return depthResult.Result();
    }

    // Clear old swapchain

    delete this->depthImage;

    for (VkImageView& view : this->swapchainImageViews) {
        vkDestroyImageView(this->device->device, view, nullptr);
    }

    vkDestroySwapchainKHR(this->device->device, this->swapchain, nullptr);

    // Overwrite existing data

    Device::SwapchainData swapchain = swapchainResult.Unwrap();

    this->capabilities = dynamics.capabilities;
    this->extent = dynamics.extent;

    this->swapchain = swapchain.swapchain;
    this->swapchainImages = swapchain.images;
    this->swapchainImageViews = swapchain.views;

    this->depthImage = depthResult.Unwrap();

    return Result::Success;
}

}
