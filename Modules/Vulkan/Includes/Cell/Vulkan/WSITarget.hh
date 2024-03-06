// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Collection/List.hh>
#include <Cell/Shell/Shell.hh>
#include <Cell/Vulkan/Image.hh>
#include <Cell/Vulkan/RenderTarget.hh>

namespace Cell::Vulkan {

// Vulkan WSI implementation.
class WSITarget : public IRenderTarget {
friend Device;

public:
    CELL_FUNCTION ~WSITarget();

    CELL_FUNCTION Result Recreate();

    CELL_FUNCTION uint32_t GetFrameCounter();
    CELL_FUNCTION VkImageView GetSwapchainImageViewForCurrentIndex();
    CELL_FUNCTION VkImage GetSwapchainImageForCurrentIndex();
    CELL_FUNCTION Image* GetDepthImage();

    CELL_FUNCTION Wrapped<AcquiredImage, Result> AcquireNext() override;
    CELL_FUNCTION Result Present() override;

    CELL_FUNCTION VkExtent2D GetExtent() override;
    CELL_FUNCTION VkFormat GetColorFormat() override;
    CELL_FUNCTION uint32_t GetImageCount() override;
    CELL_FUNCTION uint32_t GetCurrentImageIndex() override;
    CELL_FUNCTION VkImage GetColorImage(const uint32_t index) override;
    CELL_FUNCTION VkImageView GetColorImageView(const uint32_t index) override;
    CELL_FUNCTION VkImageView GetDepthImageView(const uint32_t index) override;

    CELL_NON_COPYABLE(WSITarget)

private:
    CELL_FUNCTION_INTERNAL CELL_INLINE WSITarget(VkSurfaceKHR s, const uint8_t d, const VkSurfaceCapabilitiesKHR& c,
                                                 const VkSurfaceFormatKHR f, const VkPresentModeKHR p, const VkExtent2D e,
                                                 VkSwapchainKHR sc, Collection::List<VkImage>& i,
                                                 Collection::List<VkImageView>& v, Image*& di,
                                                 Collection::List<VkSemaphore>& ia, Collection::List<VkSemaphore>& rf,
                                                 Collection::List<VkFence>& iff, Device* de, Shell::IShell* sh)
        : surface(s), capabilities(c), format(f), mode(p), extent(e),
          swapchain(sc), depth(d), swapchainImages(i), swapchainImageViews(v),
          imageAvailable(ia), renderFinished(rf), inFlightFrames(iff), depthImage(di),
          device(de), shell(sh) { }

    VkSurfaceKHR surface;
    VkSurfaceCapabilitiesKHR capabilities;
    VkSurfaceFormatKHR format;
    VkPresentModeKHR mode;
    VkExtent2D extent;
    VkSwapchainKHR swapchain;
    uint32_t depth;

    Collection::List<VkImage> swapchainImages;
    Collection::List<VkImageView> swapchainImageViews;

    Collection::List<VkSemaphore> imageAvailable;
    Collection::List<VkSemaphore> renderFinished;
    Collection::List<VkFence> inFlightFrames;

    Image* depthImage;

    Vulkan::Device* device;
    Shell::IShell* shell;

    uint32_t renderImageIndex = 0;
    uint32_t renderFrameCounter = 0;
};

}
