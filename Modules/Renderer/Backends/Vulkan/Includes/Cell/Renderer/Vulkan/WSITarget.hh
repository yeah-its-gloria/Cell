// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Collection/List.hh>
#include <Cell/Renderer/Vulkan/Image.hh>
#include <Cell/Renderer/Vulkan/RenderTarget.hh>
#include <Cell/Shell/Shell.hh>

namespace Cell::Renderer::Vulkan {

// Vulkan WSI implementation.
class WSITarget : public IRenderTarget {
friend Device;

public:
    CELL_FUNCTION ~WSITarget();
    CELL_FUNCTION Result Recreate();
    CELL_FUNCTION Wrapped<AcquiredImage, Result> AcquireNext() override;
    CELL_FUNCTION Result Present() override;

    CELL_FUNCTION_TEMPLATE uint32_t GetFrameCounter()
        { return this->renderFrameCounter; }

    CELL_FUNCTION_TEMPLATE VkImageView GetSwapchainImageViewForCurrentIndex()
        { return this->swapchainImageViews[this->renderImageIndex]; }

    CELL_FUNCTION_TEMPLATE VkImage GetSwapchainImageForCurrentIndex()
        { return this->swapchainImages[this->renderImageIndex]; }

    CELL_FUNCTION_TEMPLATE Image* GetDepthImage()
        { return this->depthImage; }

    CELL_FUNCTION_TEMPLATE VkExtent2D GetExtent() override
        { return this->extent; }

    CELL_FUNCTION_TEMPLATE VkFormat GetColorFormat() override
        { return this->format.format; }

    CELL_FUNCTION_TEMPLATE uint32_t GetImageCount() override
        { return this->depth; }

    CELL_FUNCTION_TEMPLATE uint32_t GetCurrentImageIndex() override
        { return this->renderImageIndex; }

    CELL_FUNCTION_TEMPLATE VkImage GetColorImage(const uint32_t index) override
        { return this->swapchainImages[index]; }

    CELL_FUNCTION_TEMPLATE VkImageView GetColorImageView(const uint32_t index) override
        { return this->swapchainImageViews[index]; }

    CELL_FUNCTION_TEMPLATE VkImageView GetDepthImageView(const uint32_t index) override
        { (void)(index); return this->depthImage->GetViewHandle(); }

private:
    CELL_FUNCTION_INTERNAL WSITarget(VkSurfaceKHR s, const uint8_t d, const VkSurfaceCapabilitiesKHR& c,
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
    uint8_t depth;

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
