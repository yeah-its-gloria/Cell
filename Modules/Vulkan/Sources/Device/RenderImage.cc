// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/Pipeline.hh>
#include <Cell/Vulkan/RenderTarget.hh>

namespace Cell::Vulkan {

Result Device::RenderImage(IRenderTarget* target, VkCommandBuffer CELL_NONNULL buffer) {
    Wrapped <AcquiredImage, Result> acquiredResult = target->AcquireNext();
    if (!acquiredResult.IsValid()) {
        return acquiredResult.Result();
    }

    AcquiredImage image = acquiredResult.Unwrap();

    const VkPipelineStageFlags stageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    const VkSubmitInfo submitInfo = {
        .sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext                = nullptr,

        .waitSemaphoreCount   = image.usesSync ? (uint32_t)1 : 0,
        .pWaitSemaphores      = &image.available,
        .pWaitDstStageMask    = &stageMask,

        .commandBufferCount   = 1,
        .pCommandBuffers      = &buffer,

        .signalSemaphoreCount = image.usesSync ? (uint32_t)1 : 0,
        .pSignalSemaphores    = &image.rendered
    };

    VkResult vkResult = vkQueueSubmit(this->deviceQueueGraphics, 1, &submitInfo, image.usesSync ? image.inFlight : nullptr);
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
        System::Panic("vkQueueSubmit failed");
    }
    }

    Result result = target->Present();
    if (result != Result::Success) {
        return result;
    }

    vkResult = vkDeviceWaitIdle(this->device);
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

    return Result::Success;
}

}
