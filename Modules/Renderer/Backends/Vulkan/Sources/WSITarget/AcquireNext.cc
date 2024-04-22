// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Renderer/Vulkan/WSITarget.hh>

namespace Cell::Renderer::Vulkan {

Wrapped<AcquiredImage, Result> WSITarget::AcquireNext() {
    VkResult result = vkWaitForFences(this->device->device, 1, &this->inFlightFrames[this->renderFrameCounter], VK_TRUE, UINT64_MAX);
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

    default: {
        System::Panic("vkWaitForFences failed");
    }
    }

    result = vkAcquireNextImageKHR(this->device->device, this->swapchain, UINT64_MAX, this->imageAvailable[this->renderFrameCounter], nullptr, &this->renderImageIndex);
    switch (result) {
    case VK_SUCCESS:
    case VK_SUBOPTIMAL_KHR: {
        break;
    }

    case VK_ERROR_OUT_OF_DATE_KHR: {
        return Result::Suboptimal;
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
        System::Panic("vkAcquireNextImageKHR failed");
    }
    }

    result = vkResetFences(this->device->device, 1, &this->inFlightFrames[this->renderFrameCounter]);
    switch (result) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        return Result::OutOfDeviceMemory;
    }

    default: {
        System::Panic("vkResetFences failed");
    }
    }

    return AcquiredImage {
        this->swapchainImages[this->renderImageIndex],
        true,
        this->imageAvailable[this->renderFrameCounter],
        this->renderFinished[this->renderFrameCounter],
        this->inFlightFrames[this->renderFrameCounter]
    };
}

}
