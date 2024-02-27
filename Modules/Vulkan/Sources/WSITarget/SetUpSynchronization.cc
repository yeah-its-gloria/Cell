// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/WSITarget.hh>

namespace Cell::Vulkan {

Result WSITarget::SetUpSynchronization() {
    if (this->swapchain == nullptr) {
        return Result::InvalidState;
    }

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

    this->imageAvailable.SetCount(this->swapchainDepth);
    this->renderFinished.SetCount(this->swapchainDepth);
    this->inFlightFrames.SetCount(this->swapchainDepth);

    VkResult result = VK_ERROR_UNKNOWN;
    for (size_t i = 0; i < this->swapchainDepth; i++) {
        result = vkCreateSemaphore(this->device->device, &semaphoreInfo, nullptr, &this->imageAvailable[i]);
        if (result != VK_SUCCESS) {
            break;
        }

        result = vkCreateSemaphore(this->device->device, &semaphoreInfo, nullptr, &this->renderFinished[i]);
        if (result != VK_SUCCESS) {
            break;
        }

        result = vkCreateFence(this->device->device, &fenceInfo, nullptr, &this->inFlightFrames[i]);
        if (result != VK_SUCCESS) {
            break;
        }
    }

    switch (result) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        for (VkSemaphore semaphore : this->imageAvailable) {
            if (semaphore != nullptr) {
                vkDestroySemaphore(this->device->device, semaphore, nullptr);
            }
        }

        for (VkSemaphore semaphore : this->renderFinished) {
            if (semaphore != nullptr) {
                vkDestroySemaphore(this->device->device, semaphore, nullptr);
            }
        }

        for (VkFence fence : this->inFlightFrames) {
            if (fence != nullptr) {
                vkDestroyFence(this->device->device, fence, nullptr);
            }
        }

        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        for (VkSemaphore semaphore : this->imageAvailable) {
            if (semaphore != nullptr) {
                vkDestroySemaphore(this->device->device, semaphore, nullptr);
            }
        }

        for (VkSemaphore semaphore : this->renderFinished) {
            if (semaphore != nullptr) {
                vkDestroySemaphore(this->device->device, semaphore, nullptr);
            }
        }

        for (VkFence fence : this->inFlightFrames) {
            if (fence != nullptr) {
                vkDestroyFence(this->device->device, fence, nullptr);
            }
        }

        return Result::OutOfDeviceMemory;
    }

    default: {
        System::Panic("vkCreateSemaphore/vkCreateFence failed");
    }
    }

    return Result::Success;
}

}
