// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/Collection/Array.hh>
#include <Cell/Renderer/Vulkan/Buffer.hh>
#include <Cell/Renderer/Vulkan/CommandBuffer.hh>
#include <Cell/Renderer/Vulkan/Image.hh>

namespace Cell::Renderer::Vulkan {

Result Image::CopyDataFromBuffer(Buffer* buffer) {
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

    const CommandParameters::InsertBarrier barrierFirstParameters = {
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

        .imageExtent.width               = this->width,
        .imageExtent.height              = this->height,
        .imageExtent.depth               = this->depth
    };

    const CommandParameters::CopyBufferToImage copyParameters = {
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

    const CommandParameters::InsertBarrier barrierSecondParameters = {
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

    const Collection::List<const Command> commands = {
        { CommandType::InsertBarrier,     &barrierFirstParameters },
        { CommandType::CopyBufferToImage, &copyParameters },
        { CommandType::InsertBarrier,     &barrierSecondParameters }
    };

    Wrapped<CommandBuffer*, Result> commandBufferResult = this->device->CreateCommandBuffer();
    if (!commandBufferResult.IsValid()) {
        return commandBufferResult.Result();
    }

    ScopedObject<CommandBuffer> commandBuffer = commandBufferResult.Unwrap();

    const Result result = commandBuffer->WriteSinglePass(commands);
    if (result != Result::Success) {
        return result;
    }

    return commandBuffer->Submit();
}

}
