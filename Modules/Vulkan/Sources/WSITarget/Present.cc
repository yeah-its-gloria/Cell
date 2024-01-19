// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/WSITarget.hh>

namespace Cell::Vulkan {

Result WSITarget::Present() {
    const VkPresentInfoKHR presentationInfo = {
        .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext              = nullptr,

        .waitSemaphoreCount = 1,
        .pWaitSemaphores    = &this->renderFinished[this->renderFrameCounter],

        .swapchainCount     = 1,
        .pSwapchains        = &this->swapchain,
        .pImageIndices      = &this->renderImageIndex,

        .pResults           = nullptr
    };

    const VkResult result = vkQueuePresentKHR(this->instance->deviceQueueGraphics, &presentationInfo);
    switch (result) {
    case VK_SUCCESS: {
        this->renderFrameCounter = (this->renderFrameCounter + 1) % this->swapchainDepth;
        break;
    }

    case VK_SUBOPTIMAL_KHR:
    case VK_ERROR_OUT_OF_DATE_KHR: {
        this->renderFrameCounter = (this->renderFrameCounter + 1) % this->swapchainDepth;
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
        System::Panic("vkQueuePresentKHR failed");
    }
    }

    return Result::Success;
}

}
