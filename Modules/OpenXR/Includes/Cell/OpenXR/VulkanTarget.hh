// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Collection/List.hh>
#include <Cell/OpenXR/Instance.hh>
#include <Cell/Vulkan/RenderTarget.hh>

namespace Cell::OpenXR {

// Vulkan XR swapchain implementation.
class VulkanTarget final : public Vulkan::IRenderTarget {
friend Instance;

public:
    CELL_FUNCTION ~VulkanTarget() override;

    CELL_FUNCTION Wrapped<Vulkan::AcquiredImage, Vulkan::Result> AcquireNext() override;
    CELL_FUNCTION Vulkan::Result Present() override;

    CELL_INLINE VkExtent2D GetExtent() override { return { this->viewConfig->recommendedImageRectWidth, this->viewConfig->recommendedImageRectHeight }; }
    CELL_INLINE VkFormat GetColorFormat() override { return VK_FORMAT_R8G8B8A8_SRGB; }
    CELL_INLINE uint32_t GetImageCount() override { return this->swapchainColorImages.GetCount(); }
    CELL_INLINE uint32_t GetCurrentImageIndex() override { return this->imageIndex; }
    CELL_INLINE VkImage GetColorImage(const uint32_t index) override { return this->swapchainColorImages[index].image; }
    CELL_INLINE VkImageView GetColorImageView(const uint32_t index) override { return this->swapchainColorImageViews[index]; }
    CELL_INLINE VkImageView GetDepthImageView(const uint32_t index) override { return this->swapchainDepthImageViews[index]; }

private:
    VulkanTarget(OpenXR::Instance* instance, XrViewConfigurationView* viewConfig, XrView* view) : instance(instance), viewConfig(viewConfig), view(view) { }

    CELL_FUNCTION Result CreateSwapchains();

    OpenXR::Instance* instance;

    XrViewConfigurationView* viewConfig;
    XrView* view;

    XrSwapchain colorSwapchain = nullptr;
    XrSwapchain depthSwapchain = nullptr;

    Collection::List<XrSwapchainImageVulkanKHR> swapchainColorImages;
    Collection::List<XrSwapchainImageVulkanKHR> swapchainDepthImages;

    Collection::List<VkImageView> swapchainColorImageViews;
    Collection::List<VkImageView> swapchainDepthImageViews;

    XrCompositionLayerDepthInfoKHR depthView = { XR_TYPE_COMPOSITION_LAYER_DEPTH_INFO_KHR, nullptr, { nullptr, { { 0, 0 }, { 0, 0 } }, 0 }, 0.f, 1.f, 0.01f, 100.f };
    XrCompositionLayerProjectionView projectionView = { XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW, &this->depthView, { { 0.f, 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f } }, { 0.f, 0.f, 0.f, 0.f },
                                                        { nullptr, { { 0, 0 }, { 0, 0 } }, 0 } };

    uint32_t imageIndex = 0;
};


}
