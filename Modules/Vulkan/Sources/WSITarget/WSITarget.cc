// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/WSITarget.hh>

namespace Cell::Vulkan {

WSITarget::~WSITarget() {
    for (VkFence fence : this->inFlightFrames) {
        vkDestroyFence(this->device->device, fence, nullptr);
    }

    for (VkSemaphore semaphore : this->renderFinished) {
        vkDestroySemaphore(this->device->device, semaphore, nullptr);
    }

    for (VkSemaphore semaphore : this->imageAvailable) {
        vkDestroySemaphore(this->device->device, semaphore, nullptr);
    }

    delete this->depthImage;

    for (VkImageView view : this->swapchainImageViews) {
        vkDestroyImageView(this->device->device, view, nullptr);
    }

    vkDestroySwapchainKHR(this->device->device, this->swapchain, nullptr);
    vkDestroySurfaceKHR(this->device->instance->instance, this->surface, nullptr);
}

uint32_t WSITarget::GetFrameCounter() {
    return this->renderFrameCounter;
}

VkImageView WSITarget::GetSwapchainImageViewForCurrentIndex() {
    return this->swapchainImageViews[this->renderImageIndex];
}

VkImage WSITarget::GetSwapchainImageForCurrentIndex() {
    return this->swapchainImages[this->renderImageIndex];
}

Image* WSITarget::GetDepthImage() {
    return this->depthImage;
}

VkExtent2D WSITarget::GetExtent() {
    return this->extent;
}

VkFormat WSITarget::GetColorFormat() {
    return this->format.format;
}

uint32_t WSITarget::GetImageCount() {
    return this->depth;
}

uint32_t WSITarget::GetCurrentImageIndex() {
    return this->renderImageIndex;
}

VkImage WSITarget::GetColorImage(const uint32_t index) {
    return this->swapchainImages[index];
}

VkImageView WSITarget::GetColorImageView(const uint32_t index) {
    return this->swapchainImageViews[index];
}

VkImageView WSITarget::GetDepthImageView(const uint32_t index) {
    (void)(index); return this->depthImage->GetViewHandle();
}

}
