// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/List.hh>
#include <Cell/Vulkan/Image.hh>
#include <Cell/Vulkan/RenderTarget.hh>

namespace Cell::Vulkan {

// Vulkan WSI implementation.
class WSITarget final : public IRenderTarget {
friend Instance;

public:
// RenderTarget
    CELL_FUNCTION Wrapped<AcquiredImage, Result> AcquireNext() override;
    CELL_FUNCTION Result Present() override;

    CELL_INLINE   VkExtent2D GetExtent() override { return this->extent; }
    CELL_INLINE   VkFormat GetColorFormat() override { return this->format.format; }
    CELL_INLINE   uint32_t GetImageCount() override { return this->swapchainDepth; }
    CELL_INLINE   uint32_t GetCurrentImageIndex() override { return this->renderImageIndex; }
    CELL_INLINE   VkImage GetColorImage(const uint32_t index) override { return this->swapchainImages[index]; }
    CELL_INLINE   VkImageView GetColorImageView(const uint32_t index) override { return this->swapchainImageViews[index]; }

    CELL_INLINE   VkImageView GetDepthImageView(const uint32_t index) override {
        (void)(index);
        return this->depthImage->GetViewHandle();
    }

// WSITarget
    CELL_FUNCTION ~WSITarget();

    CELL_FUNCTION Result SetUpRendering();
    CELL_FUNCTION Result Recreate();

    CELL_INLINE   uint32_t GetFrameCount() { return this->renderFrameCounter; }
    CELL_INLINE   VkImageView GetSwapchainImageViewForCurrentIndex() { return this->swapchainImageViews[this->renderImageIndex]; }
    CELL_INLINE   VkImage GetSwapchainImageForCurrentIndex() { return this->swapchainImages[this->renderImageIndex]; }
    CELL_INLINE   Image* GetDepthImage() { return this->depthImage; }

private:
    WSITarget(Vulkan::Instance* instance, System::IPlatform& platform, VkSurfaceKHR surface, VkSurfaceFormatKHR format, VkPresentModeKHR presentMode, const uint32_t swapchainDepth = 4)
        : instance(instance), platform(platform), surface(surface), format(format), presentMode(presentMode), swapchainDepth(swapchainDepth) { }

    CELL_FUNCTION Result RetrieveProperties();
    CELL_FUNCTION Result CreateSwapchain();
    CELL_FUNCTION Result PrepareSwapchain();
    CELL_FUNCTION Result SetUpDepthBuffer();
    CELL_FUNCTION Result SetUpSynchronization();

    Vulkan::Instance* instance;
    System::IPlatform& platform;

    VkSurfaceKHR surface;
    VkSurfaceCapabilitiesKHR capabilities = { 0, 0, { 0, 0 }, { 0, 0 }, { 0, 0 }, 0, 0, (VkSurfaceTransformFlagBitsKHR)0, 0, 0 };
    VkSurfaceFormatKHR format;
    VkPresentModeKHR presentMode;
    VkExtent2D extent = { 0, 0 };

    VkSwapchainKHR swapchain = nullptr;
    List<VkImage> swapchainImages;
    List<VkImageView> swapchainImageViews;

    List<VkSemaphore> imageAvailable;
    List<VkSemaphore> renderFinished;
    List<VkFence> inFlightFrames;

    uint32_t renderImageIndex = 0;
    uint32_t renderFrameCounter = 0;

    uint32_t swapchainDepth;

    Image* depthImage = nullptr;
};

}
