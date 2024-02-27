// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/Collection/Single.hh>
#include <Cell/Vulkan/CommandBuffer.hh>
#include <Cell/Vulkan/WSITarget.hh>

namespace Cell::Vulkan {

Result WSITarget::PrepareSwapchain() {
    Wrapped<CommandBuffer*, Result> commandBufferResult = this->device->CreateCommandBuffer();
    if (!commandBufferResult.IsValid()) {
        return commandBufferResult.Result();
    }

    ScopedObject commandBuffer = commandBufferResult.Unwrap();

    Result result = commandBuffer->Begin();
    if (result != Result::Success) {
        return result;
    }

    for (VkImage image : this->swapchainImages) {
        const VkImageMemoryBarrier swapchainBarrierImage = {
            .sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext                           = nullptr,
            .srcAccessMask                   = VK_ACCESS_NONE,
            .dstAccessMask                   = VK_ACCESS_NONE,
            .oldLayout                       = VK_IMAGE_LAYOUT_UNDEFINED,
            .newLayout                       = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            .srcQueueFamilyIndex             = 0,
            .dstQueueFamilyIndex             = 0,
            .image                           = image,
            .subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
            .subresourceRange.baseMipLevel   = 0,
            .subresourceRange.levelCount     = 1,
            .subresourceRange.baseArrayLayer = 0,
            .subresourceRange.layerCount     = 1
        };

        const CommandParameters::InsertBarrier swapchainBarrier = {
            .stageMaskSource      = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .stageMaskDestination = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
            .dependencyFlags      = 0,
            .barrierCount         = 0,
            .barriers             = nullptr,
            .bufferCount          = 0,
            .buffers              = nullptr,
            .imageCount           = 1,
            .images               = &swapchainBarrierImage
        };

        const Command command = { CommandType::InsertBarrier, &swapchainBarrier };
        result = commandBuffer->Write(Collection::Single(command));
        if (result != Result::Success) {
            return result;
        }
    }

    result = commandBuffer->End();
    if (result != Result::Success) {
        return result;
    }

    return commandBuffer->Submit();
}

}
