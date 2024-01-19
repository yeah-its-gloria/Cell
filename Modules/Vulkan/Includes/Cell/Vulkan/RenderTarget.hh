// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Vulkan/Instance.hh>

namespace Cell::Vulkan {

// Information about an acquired image.
struct AcquiredImage {
    VkImage image;

    bool usesSync;
    VkSemaphore available;
    VkSemaphore rendered;
    VkFence inFlight;
};

// Render target interface; this is used to perform presentation.
//
// Known implementations:
// CellOpenXR: VulkanTarget
// CellVulkan: WSITarget
class IRenderTarget : public Object {
public:
    // Common destructor.
    virtual ~IRenderTarget() = default;

    // Acquires the image at the given index.
    virtual Wrapped<AcquiredImage, Result> AcquireNext() = 0;

    // Presents the last acquired image.
    virtual Result Present() = 0;

    // Retrieves the extent of either the target in case of regular rendering, or a single image in case of stereo rendering.
    virtual VkExtent2D GetExtent() = 0;

    // Retrieves the display color format.
    virtual VkFormat GetColorFormat() = 0;

    // Retrieves the currently to be drawn image index.
    virtual uint32_t GetCurrentImageIndex() = 0;

    // Retrieves the number of images managed by the target.
    virtual uint32_t GetImageCount() = 0;

    // Retrieves a bare color image for the given index.
    virtual VkImage GetColorImage(const uint32_t index) = 0;

    // Retrieves a bare color image view for the given index.
    virtual VkImageView GetColorImageView(const uint32_t index) = 0;

    // Retrieves a bare depth image view for the given index.
    virtual VkImageView GetDepthImageView(const uint32_t index) = 0;
};

}
