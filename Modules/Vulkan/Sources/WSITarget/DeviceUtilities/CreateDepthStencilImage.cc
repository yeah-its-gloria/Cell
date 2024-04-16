// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/Collection/Single.hh>
#include <Cell/Vulkan/Image.hh>
#include <Cell/Vulkan/CommandBuffer.hh>

namespace Cell::Vulkan {

Wrapped<Image*, Result> Device::CreateDepthStencilImage(const VkExtent2D& extent) {
    Wrapped<Image*, Result> imageResult = this->CreateImage(extent.width,
                                                            extent.height,
                                                            VK_FORMAT_D32_SFLOAT_S8_UINT,
                                                            VK_IMAGE_TILING_OPTIMAL,
                                                            (VkImageAspectFlagBits)(VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT),
                                                            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
    if (!imageResult.IsValid()) {
        return imageResult.Result();
    }

    Image* depthImage = imageResult.Unwrap();

    Wrapped<CommandBuffer*, Result> commandBufferResult = this->CreateCommandBuffer();
    if (!commandBufferResult.IsValid()) {
        return commandBufferResult.Result();
    }

    ScopedObject<CommandBuffer> commandBuffer = commandBufferResult.Unwrap();

    const VkImageMemoryBarrier transition = {
        .sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .pNext                           = nullptr,
        .srcAccessMask                   = VK_ACCESS_NONE,
        .dstAccessMask                   = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
        .oldLayout                       = VK_IMAGE_LAYOUT_UNDEFINED,
        .newLayout                       = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
        .srcQueueFamilyIndex             = 0,
        .dstQueueFamilyIndex             = 0,
        .image                           = depthImage->GetHandle(),
        .subresourceRange.aspectMask     = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT,
        .subresourceRange.baseMipLevel   = 0,
        .subresourceRange.levelCount     = 1,
        .subresourceRange.baseArrayLayer = 0,
        .subresourceRange.layerCount     = 1
    };

    const CommandParameters::InsertBarrier transitionParam = {
        .stageMaskSource      = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        .stageMaskDestination = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
        .dependencyFlags      = 0,
        .barrierCount         = 0,
        .barriers             = nullptr,
        .bufferCount          = 0,
        .buffers              = nullptr,
        .imageCount           = 1,
        .images               = &transition
    };

    const Command command = { CommandType::InsertBarrier, &transitionParam };
    Result result = commandBuffer->WriteSinglePass(Collection::Single(command));
    if (result != Result::Success) {
        return result;
    }

    result = commandBuffer->Submit();
    if (result != Result::Success) {
        delete depthImage;
        return result;
    }

    return depthImage;
}

}
