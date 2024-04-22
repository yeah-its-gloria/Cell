// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Renderer/Vulkan/CommandBuffer.hh>
#include <Cell/Renderer/Vulkan/Pipeline.hh>

namespace Cell::Renderer::Vulkan {

Result CommandBuffer::Submit(IRenderTarget* target) {
    CELL_ASSERT(this->recordState == RecordState::Recorded);
    CELL_ASSERT(this->queueRef == this->device->deviceQueueGraphics);

    Wrapped<AcquiredImage, Result> acquiredResult = target->AcquireNext();
    if (!acquiredResult.IsValid()) {
        return acquiredResult.Result();
    }

    AcquiredImage image = acquiredResult.Unwrap();

    const VkPipelineStageFlags stageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    const VkSubmitInfo submitInfo = {
        .sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext                = nullptr,

        .waitSemaphoreCount   = (uint32_t)(image.usesSync ? 1 : 0),
        .pWaitSemaphores      = &image.available,
        .pWaitDstStageMask    = &stageMask,

        .commandBufferCount   = 1,
        .pCommandBuffers      = &this->buffer,

        .signalSemaphoreCount = (uint32_t)(image.usesSync ? 1 : 0),
        .pSignalSemaphores    = &image.rendered
    };

    VkResult result = vkQueueSubmit(this->queueRef, 1, &submitInfo, image.usesSync ? image.inFlight : nullptr);
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
        System::Panic("vkQueueSubmit failed");
    }
    }

    const Result presentResult = target->Present();
    if (presentResult != Result::Success) {
        return presentResult;
    }

    result = vkDeviceWaitIdle(this->device->device);
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
        System::Panic("vkDeviceWaitIdle failed");
    }
    }

    return Result::Success;
}

}
