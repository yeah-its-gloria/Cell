// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Renderer/Vulkan/WSITarget.hh>

namespace Cell::Renderer::Vulkan {

Result WSITarget::Present() {
    const VkPresentInfoKHR presentInfo = {
        .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext              = nullptr,

        .waitSemaphoreCount = 1,
        .pWaitSemaphores    = &this->renderFinished[this->renderFrameCounter],

        .swapchainCount     = 1,
        .pSwapchains        = &this->swapchain,
        .pImageIndices      = &this->renderImageIndex,

        .pResults           = nullptr
    };

    const VkResult result = vkQueuePresentKHR(this->device->deviceQueueGraphics, &presentInfo);
    switch (result) {
    case VK_SUCCESS: {
        this->renderFrameCounter = (this->renderFrameCounter + 1) % this->depth;
        break;
    }

    case VK_SUBOPTIMAL_KHR:
    case VK_ERROR_OUT_OF_DATE_KHR: {
        this->renderFrameCounter = (this->renderFrameCounter + 1) % this->depth;
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
