// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Vulkan/Buffer.hh>

namespace Cell::Vulkan {

class Pipeline;

// Represents an image.
class Image : public Object {
friend Device;
friend Pipeline;

public:
    // Destructs the images.
    CELL_FUNCTION ~Image();

    // Copies the image data from a buffer. This creates its own internal command buffer.
    CELL_FUNCTION Result CopyDataFromBuffer(Buffer* CELL_NONNULL buffer);

    // Returns a handle to the underlying image.
    CELL_INLINE VkImage GetHandle() { return this->image; }

    // Returns a handle to the underlying image view.
    CELL_INLINE VkImageView GetViewHandle() { return this->view; }

    // Returns a handle to the underlying sampler.
    CELL_INLINE VkSampler GetSamplerHandle() { return this->sampler; }

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
