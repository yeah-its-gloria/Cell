// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/Vulkan/Buffer.hh>
#include <Cell/Vulkan/CommandBufferManager.hh>
#include <Cell/Vulkan/Image.hh>

namespace Cell::Vulkan {

Result Image::CopyDataFromBuffer(Buffer* buffer) {
    Wrapped<CommandBufferManager*, Result> cmdBufferManagerResult = this->instance->CreateCommandBufferManager();
    if (!cmdBufferManagerResult.IsValid()) {
        return cmdBufferManagerResult.Result();
    }

    ScopedObject<CommandBufferManager> cmdBufferManager = cmdBufferManagerResult.Unwrap();

    Result result = cmdBufferManager->CreateBuffers(1);
    if (result != Result::Success) {
        return result;
    }

    const VkImageMemoryBarrier barrierFirstData = {
        .sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .pNext                           = nullptr,

        .srcAccessMask                   = 0,
        .dstAccessMask                   = VK_ACCESS_TRANSFER_WRITE_BIT,

        .oldLayout                       = VK_IMAGE_LAYOUT_UNDEFINED,
        .newLayout                       = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,

        .srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED,

        .image                           = this->image,

        .subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
        .subresourceRange.baseMipLevel   = 0,
        .subresourceRange.levelCount     = 1,
        .subresourceRange.baseArrayLayer = 0,
        .subresourceRange.layerCount     = 1
    };

    CommandParameters::InsertBarrier barrierFirstParameters = {
        .stageMaskSource      = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        .stageMaskDestination = VK_PIPELINE_STAGE_TRANSFER_BIT,
        .dependencyFlags      = 0,
        .barrierCount         = 0,
        .barriers             = nullptr,
        .bufferCount          = 0,
        .buffers              = nullptr,
        .imageCount           = 1,
        .images               = &barrierFirstData
    };

    const VkBufferImageCopy region = {
        .bufferOffset                    = 0,
        .bufferRowLength                 = 0,
        .bufferImageHeight               = 0,

        .imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
        .imageSubresource.mipLevel       = 0,
        .imageSubresource.baseArrayLayer = 0,
        .imageSubresource.layerCount     = 1,

        .imageOffset.x                   = 0,
        .imageOffset.y                   = 0,
        .imageOffset.z                   = 0,

        .imageExtent.width               = 1024,
        .imageExtent.height              = 1024,
        .imageExtent.depth               = 1
    };

    CommandParameters::CopyBufferToImage copyParameters = {
        .source      = buffer->GetBufferHandle(),
        .destination = this->image,
        .layout      = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .regionCount = 1,
        .regions     = &region
    };

    const VkImageMemoryBarrier barrierSecondData = {
        .sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .pNext                           = nullptr,

        .srcAccessMask                   = VK_ACCESS_TRANSFER_WRITE_BIT,
        .dstAccessMask                   = VK_ACCESS_SHADER_READ_BIT,

        .oldLayout                       = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .newLayout                       = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,

        .srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED,

        .image                           = this->image,

        .subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
        .subresourceRange.baseMipLevel   = 0,
        .subresourceRange.levelCount     = 1,
        .subresourceRange.baseArrayLayer = 0,
        .subresourceRange.layerCount     = 1
    };

    CommandParameters::InsertBarrier barrierSecondParameters = {
        .stageMaskSource      = VK_PIPELINE_STAGE_TRANSFER_BIT,
        .stageMaskDestination = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        .dependencyFlags      = 0,
        .barrierCount         = 0,
        .barriers             = nullptr,
        .bufferCount          = 0,
        .buffers              = nullptr,
        .imageCount           = 1,
        .images               = &barrierSecondData
    };

    Command commands[3] = {
        { CommandType::InsertBarrier,     &barrierFirstParameters },
        { CommandType::CopyBufferToImage, &copyParameters },
        { CommandType::InsertBarrier,     &barrierSecondParameters }
    };

    result = cmdBufferManager->WriteCommandsSingle(0, commands, 3);
    if (result != Result::Success) {
        return result;
    }

    return cmdBufferManager->Submit(0);
}

}
