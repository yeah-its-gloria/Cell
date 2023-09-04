// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/Image.hh>
#include <Cell/Vulkan/WSITarget.hh>

namespace Cell::Vulkan {

WSITarget::~WSITarget() {
    if (this->inFlightFrames.GetCount() > 0) {
        for (VkFence fence : this->inFlightFrames) {
            vkDestroyFence(this->instance->device, fence, nullptr);
        }
    }

    if (this->renderFinished.GetCount() > 0) {
        for (VkSemaphore semaphore : this->renderFinished) {
            vkDestroySemaphore(this->instance->device, semaphore, nullptr);
        }
    }

    if (this->depthImage != nullptr) {
        delete this->depthImage;
    }

    if (this->imageAvailable.GetCount() > 0) {
        for (VkSemaphore semaphore : this->imageAvailable) {
            vkDestroySemaphore(this->instance->device, semaphore, nullptr);
        }
    }

    if (this->swapchain != nullptr) {
        for (VkImageView view : this->swapchainImageViews) {
            vkDestroyImageView(this->instance->device, view, nullptr);
        }

        vkDestroySwapchainKHR(this->instance->device, this->swapchain, nullptr);
    }

    vkDestroySurfaceKHR(this->instance->instance, this->surface, nullptr);
}

}
