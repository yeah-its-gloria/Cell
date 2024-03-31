// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Vulkan/Buffer.hh>

namespace Cell::Vulkan {

// Represents an image.
class Image : public NoCopyObject {
friend Device;
friend class Pipeline;

public:
    // Destructs the images.
    CELL_FUNCTION ~Image();

    // Copies the image data from a buffer. This creates its own internal command buffer.
    CELL_FUNCTION Result CopyDataFromBuffer(Buffer* CELL_NONNULL buffer);

    // Returns a handle to the underlying image.
    CELL_FUNCTION inline VkImage GetHandle() { return this->image; }

    // Returns a handle to the underlying image view.
    CELL_FUNCTION inline VkImageView GetViewHandle() { return this->view; }

    // Returns a handle to the underlying sampler.
    CELL_FUNCTION inline VkSampler GetSamplerHandle() { return this->sampler; }

private:
    CELL_FUNCTION_INTERNAL Image(Device* dev,
                                 VkImage image,
                                 VkDeviceMemory mem,
                                 VkImageView view,
                                 VkSampler sampler,
                                 const uint32_t width,
                                 const uint32_t height,
                                 const uint32_t depth)
        : device(dev), image(image), memory(mem), view(view), sampler(sampler), width(width), height(height), depth(depth) { }

    Device* device;

    VkImage image;
    VkDeviceMemory memory;
    VkImageView view;
    VkSampler sampler;

    const uint32_t width;
    const uint32_t height;
    const uint32_t depth;
};

}
