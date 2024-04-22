// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <vulkan/vulkan.h>

namespace Cell::Renderer::Vulkan::CommandParameters {

// Copies the data of one buffer to another buffer.
struct CopyBuffer {
    VkBuffer source;
    VkBuffer destination;
    const uint32_t regionCount;
    const VkBufferCopy* regions;
};

// Copies the data of one buffer to an image.
struct CopyBufferToImage {
    VkBuffer source;
    VkImage destination;
    const VkImageLayout layout;
    const uint32_t regionCount;
    const VkBufferImageCopy* regions;
};

// Inserts a "barrier" - information storing how data should be treated.
struct InsertBarrier {
    const VkPipelineStageFlags stageMaskSource;
    const VkPipelineStageFlags stageMaskDestination;
    const VkDependencyFlags dependencyFlags;
    const uint32_t barrierCount;
    const VkMemoryBarrier* barriers;
    const uint32_t bufferCount;
    const VkBufferMemoryBarrier* buffers;
    const uint32_t imageCount;
    const VkImageMemoryBarrier* images;
};

}
