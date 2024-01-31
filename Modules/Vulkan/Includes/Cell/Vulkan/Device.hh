// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Shell/Shell.hh>
#include <Cell/Vulkan/Instance.hh>

namespace Cell::Vulkan {
class Buffer;
class CommandBufferManager;
class Image;
class IRenderTarget;
class Pipeline;
class WSITarget;

enum class QueueType : uint8_t {
    Graphics,
    Transfer
};

enum class Stage : uint8_t {
    Vertex,
    Fragment
};

class Device : public Object {
friend Buffer;
friend CommandBufferManager;
friend Image;
friend Instance;
friend Pipeline;
friend WSITarget;

#if CELL_MODULES_OPENXR_AVAILABLE
friend OpenXR::Instance;
friend OpenXR::VulkanTarget;
#endif

public:
    // Destructs the device, alongside all resources created with it.
    CELL_FUNCTION ~Device();

    // Creates a managed buffer.
    CELL_FUNCTION Wrapped<Buffer*, Result> CreateBuffer(
        const size_t size,
        const VkBufferUsageFlags usage,
        const VkMemoryPropertyFlags memoryType,
        const VkSharingMode shareMode = VK_SHARING_MODE_EXCLUSIVE
    );

    // Creates a managed image.
    CELL_FUNCTION Wrapped<Image*, Result> CreateImage(
        const uint32_t width,
        const uint32_t height,
        const VkFormat format = VK_FORMAT_R8G8B8A8_SRGB,
        const VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL,
        const VkImageAspectFlags viewAspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        const VkImageUsageFlags usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT
    );

    // Creates a command buffer manager.
    CELL_FUNCTION Wrapped<CommandBufferManager*, Result> CreateCommandBufferManager(const QueueType queue = QueueType::Graphics, const bool resetIndividually = false);

    // Creates a managed pipeline.
    CELL_FUNCTION Wrapped<Pipeline*, Result> CreatePipeline(IRenderTarget* CELL_NONNULL target);

    // Rendering utility - Renders to the next swapchain image index with the given buffer.
    CELL_FUNCTION Result RenderImage(IRenderTarget* CELL_NONNULL target, VkCommandBuffer CELL_NONNULL buffer);

    // Creates a WSI render target.
    CELL_FUNCTION Wrapped<WSITarget*, Result> CreateWSITarget(Shell::IShell* CELL_NONNULL shell);

private:
    CELL_INLINE Device(VkPhysicalDevice physDev,
                       uint32_t physGraphics,
                       uint32_t physTransfer,
                       VkPhysicalDeviceMemoryProperties prop,
                       VkDevice dev,
                       VkQueue graphics,
                       VkQueue transfer,
                       VkInstance inst,
                       PFN_vkCmdBeginRenderingKHR br,
                       PFN_vkCmdEndRenderingKHR er,
                       PFN_vkCmdSetCullModeEXT scm)
        : physicalDevice(physDev),
          physicalDeviceQueueGraphics(physGraphics),
          physicalDeviceQueueTransfer(physTransfer),
          physicalDeviceProperties(prop),
          device(dev),
          deviceQueueGraphics(graphics),
          deviceQueueTransfer(transfer),
          instance(inst),
          beginRendering(br),
          endRendering(er),
          setCullMode(scm) { }

    CELL_FUNCTION Result CreateImageView(
        VkImageView& view,
        VkImage CELL_NONNULL image,
        const VkFormat format,
        const VkImageViewType type = VK_IMAGE_VIEW_TYPE_2D,
        const VkImageAspectFlagBits aspectMask = VK_IMAGE_ASPECT_COLOR_BIT
    );

    CELL_FUNCTION_INTERNAL uint32_t GetMemoryTypeIndex(VkBuffer buffer, const VkMemoryPropertyFlags type);
    CELL_FUNCTION_INTERNAL uint32_t GetMemoryTypeIndex(VkImage image, const VkMemoryPropertyFlags type);

    VkPhysicalDevice physicalDevice;
    uint32_t physicalDeviceQueueGraphics;
    uint32_t physicalDeviceQueueTransfer;

    VkPhysicalDeviceMemoryProperties physicalDeviceProperties;

    VkDevice device;
    VkQueue deviceQueueGraphics;
    VkQueue deviceQueueTransfer;

    VkInstance instance;
    PFN_vkCmdBeginRenderingKHR beginRendering;
    PFN_vkCmdEndRenderingKHR endRendering;
    PFN_vkCmdSetCullModeEXT setCullMode;
};

};