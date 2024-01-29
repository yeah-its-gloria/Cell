// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/Vulkan/CommandBufferManager.hh>
#include <Cell/Vulkan/WSITarget.hh>

namespace Cell::Vulkan {

Result WSITarget::PrepareSwapchain() {
    Wrapped<CommandBufferManager*, Result> cmdBufferManagerResult = this->device->CreateCommandBufferManager();
    if (!cmdBufferManagerResult.IsValid()) {
        return cmdBufferManagerResult.Result();
    }

    ScopedObject<CommandBufferManager> cmdBufferManager = cmdBufferManagerResult.Unwrap();

    Result result = cmdBufferManager->CreateBuffers(1);
    if (result != Result::Success) {
        return result;
    }

    result = cmdBufferManager->BeginCommands(0);
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

        CommandParameters::InsertBarrier swapchainBarrier = {
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

        Command command = { CommandType::InsertBarrier, &swapchainBarrier };
        result = cmdBufferManager->WriteCommands(0, &command, 1);
        if (result != Result::Success) {
            return result;
        }
    }

    result = cmdBufferManager->EndCommands(0);
    if (result != Result::Success) {
        return result;
    }

    return cmdBufferManager->Submit(0);
}

}
