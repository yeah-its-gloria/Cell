// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Collection/List.hh>
#include <Cell/Shell/Shell.hh>
#include <Cell/Vulkan/Image.hh>
#include <Cell/Vulkan/RenderTarget.hh>

namespace Cell::Vulkan {

// Vulkan WSI implementation.
class WSITarget final : public IRenderTarget {
friend Instance;

public:
    CELL_FUNCTION ~WSITarget() override;

    CELL_FUNCTION Result SetUpRendering();
    CELL_FUNCTION Result Recreate();

    CELL_INLINE uint32_t GetFrameCounter() { return this->renderFrameCounter; }
    CELL_INLINE VkImageView GetSwapchainImageViewForCurrentIndex() { return this->swapchainImageViews[this->renderImageIndex]; }
    CELL_INLINE VkImage GetSwapchainImageForCurrentIndex() { return this->swapchainImages[this->renderImageIndex]; }
    CELL_INLINE Image* GetDepthImage() { return this->depthImage; }

    CELL_FUNCTION Wrapped<AcquiredImage, Result> AcquireNext() override;
    CELL_FUNCTION Result Present() override;

    CELL_INLINE VkExtent2D GetExtent() override { return this->extent; }
    CELL_INLINE VkFormat GetColorFormat() override { return this->format.format; }
    CELL_INLINE uint32_t GetImageCount() override { return this->swapchainDepth; }
    CELL_INLINE uint32_t GetCurrentImageIndex() override { return this->renderImageIndex; }
    CELL_INLINE VkImage GetColorImage(const uint32_t index) override { return this->swapchainImages[index]; }
    CELL_INLINE VkImageView GetColorImageView(const uint32_t index) override { return this->swapchainImageViews[index]; }
    CELL_INLINE VkImageView GetDepthImageView(const uint32_t index) override { (void)(index); return this->depthImage->GetViewHandle(); }

private:
    WSITarget(Vulkan::Instance* instance, Shell::IShell* shell,
              VkSurfaceKHR surface, const VkSurfaceFormatKHR format,
              const VkPresentModeKHR presentMode, const uint32_t swapchainDepth = 4) :
              instance(instance), shell(shell),
              surface(surface), format(format),
              presentMode(presentMode), swapchainDepth(swapchainDepth) { }

    CELL_FUNCTION_INTERNAL Result RetrieveProperties();
    CELL_FUNCTION_INTERNAL Result CreateSwapchain();
    CELL_FUNCTION_INTERNAL Result PrepareSwapchain();
    CELL_FUNCTION_INTERNAL Result SetUpDepthBuffer();
    CELL_FUNCTION_INTERNAL Result SetUpSynchronization();

    Vulkan::Instance* instance;
    Shell::IShell* shell;

    VkSurfaceCapabilitiesKHR capabilities = {
        .minImageCount = 0,
        .maxImageCount = 0,
        .currentExtent = { 0, 0 },
        .minImageExtent = { 0, 0 },
        .maxImageExtent = { 0, 0 },
        .maxImageArrayLayers = 0,
        .supportedTransforms = 0,
        .currentTransform = (VkSurfaceTransformFlagBitsKHR)0,
        .supportedCompositeAlpha = 0,
        .supportedUsageFlags = 0
    };

    VkSurfaceKHR surface;
    VkSurfaceFormatKHR format;
    VkPresentModeKHR presentMode;
    VkExtent2D extent = { 0, 0 };

    VkSwapchainKHR swapchain = nullptr;
    Collection::List<VkImage> swapchainImages;
    Collection::List<VkImageView> swapchainImageViews;

    Collection::List<VkSemaphore> imageAvailable;
    Collection::List<VkSemaphore> renderFinished;
    Collection::List<VkFence> inFlightFrames;

    uint32_t renderImageIndex = 0;
    uint32_t renderFrameCounter = 0;

    uint32_t swapchainDepth;

    Image* depthImage = nullptr;
};

}
