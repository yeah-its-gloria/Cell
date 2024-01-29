// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/Image.hh>
#include <Cell/Vulkan/WSITarget.hh>

namespace Cell::Vulkan {

WSITarget::~WSITarget() {
    if (this->inFlightFrames.GetCount() > 0) {
        for (VkFence fence : this->inFlightFrames) {
            vkDestroyFence(this->device->device, fence, nullptr);
        }
    }

    if (this->renderFinished.GetCount() > 0) {
        for (VkSemaphore semaphore : this->renderFinished) {
            vkDestroySemaphore(this->device->device, semaphore, nullptr);
        }
    }

    if (this->depthImage != nullptr) {
        delete this->depthImage;
    }

    if (this->imageAvailable.GetCount() > 0) {
        for (VkSemaphore semaphore : this->imageAvailable) {
            vkDestroySemaphore(this->device->device, semaphore, nullptr);
        }
    }

    if (this->swapchain != nullptr) {
        for (VkImageView view : this->swapchainImageViews) {
            vkDestroyImageView(this->device->device, view, nullptr);
        }

        vkDestroySwapchainKHR(this->device->device, this->swapchain, nullptr);
    }

    vkDestroySurfaceKHR(this->device->instance, this->surface, nullptr);
}

}
