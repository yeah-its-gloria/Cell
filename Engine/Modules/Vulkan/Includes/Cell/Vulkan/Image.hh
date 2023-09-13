// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Vulkan/Buffer.hh>

namespace Cell::Vulkan {

class Pipeline;

// Represents an image.
class Image : public Object {
    friend Instance;
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
    CELL_FUNCTION_INTERNAL Image(Instance* instance, VkImage image, VkDeviceMemory memory, VkImageView view, VkSampler sampler,
                                 const uint32_t width, const uint32_t height, const uint32_t depth)
        : instance(instance), image(image), memory(memory), view(view), sampler(sampler), width(width), height(height), depth(depth) { }

    Instance* instance;

    VkImage image;
    VkDeviceMemory memory;
    VkImageView view;
    VkSampler sampler;

    const uint32_t width;
    const uint32_t height;
    const uint32_t depth;
};

}
