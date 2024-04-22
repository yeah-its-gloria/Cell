// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Renderer/Vulkan/WSITarget.hh>

namespace Cell::Renderer::Vulkan {

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

}
