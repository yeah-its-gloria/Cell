// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Shell/Shell.hh>
#include <Cell/Vulkan/Result.hh>
#include <Cell/Wrapped.hh>
#include <vulkan/vulkan.h>

namespace Cell {

#if CELL_MODULES_OPENXR_AVAILABLE
namespace OpenXR {
class Instance;
class VulkanTarget;
}
#endif

namespace Vulkan {
class Buffer;
class CommandBufferManager;
class Image;
class Pipeline;
class IRenderTarget;
class WSITarget;

enum class QueueType : uint8_t {
    Graphics,
    Transfer
};

enum class Stage : uint8_t {
    Vertex,
    Fragment
};

class Instance : public Object {
friend Buffer;
friend CommandBufferManager;
friend Image;
friend Pipeline;
friend WSITarget;

#if CELL_MODULES_OPENXR_AVAILABLE
friend OpenXR::Instance;
friend OpenXR::VulkanTarget;
#endif

public:
    // Creates a new instance and initializes a Vulkan instance for it.
    CELL_FUNCTION static Wrapped<Instance*, Result> New();

    // Creates a new instance and initializes a Vulkan instance for it, with the given extensions.
    CELL_FUNCTION static Wrapped<Instance*, Result> New(const char* CELL_NONNULL* CELL_NONNULL extensions, const uint32_t count);

    // Destructor for instances.
    CELL_FUNCTION ~Instance();

    // Initializes the physical and logical device. Generally only needed for standalone instances.
    CELL_FUNCTION Result InitializeDevice();

    // Creates a managed buffer.
    CELL_FUNCTION Wrapped<Buffer*, Result> CreateBuffer(
        const size_t size,
        const VkBufferUsageFlags usage,
        const VkMemoryPropertyFlags memoryType,
        const VkSharingMode shareMode = VK_SHARING_MODE_EXCLUSIVE
    );

    // Creates a command buffer manager.
    CELL_FUNCTION Wrapped<CommandBufferManager*, Result> CreateCommandBufferManager(const QueueType queue = QueueType::Graphics, const bool resetIndividually = false);

    // Creates a managed image.
    CELL_FUNCTION Wrapped<Image*, Result> CreateImage(
        const uint32_t width,
        const uint32_t height,
        const VkFormat format = VK_FORMAT_R8G8B8A8_SRGB,
        const VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL,
        const VkImageAspectFlagBits viewAspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        const VkImageUsageFlagBits usage = (VkImageUsageFlagBits)(VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT)
    );

    // Creates a managed pipeline.
    CELL_FUNCTION Wrapped<Pipeline*, Result> CreatePipeline(IRenderTarget* CELL_NONNULL target);

    // Creates a WSI render target.
    CELL_FUNCTION Wrapped<WSITarget*, Result> CreateWSITarget(Shell::IShell* CELL_NONNULL shell);

    // Rendering utility - Renders to the next swapchain image index with the given buffer.
    CELL_FUNCTION Result RenderImage(IRenderTarget* CELL_NONNULL target, VkCommandBuffer CELL_NONNULL buffer);

private:
    CELL_INLINE Instance(VkInstance CELL_NONNULL instance, PFN_vkCmdBeginRenderingKHR CELL_NONNULL beginRendering, PFN_vkCmdEndRenderingKHR CELL_NONNULL endRendering)
        : instance(instance), beginRendering(beginRendering), endRendering(endRendering) { }

    CELL_FUNCTION_INTERNAL Result QueryPhysicalDevice();
    CELL_FUNCTION_INTERNAL Result CreateDevice();
    CELL_FUNCTION_INTERNAL static uint16_t ScorePhysicalDevice(VkPhysicalDevice CELL_NONNULL device);
    CELL_FUNCTION_INTERNAL uint32_t GetMemoryTypeIndex(VkBuffer buffer, const VkMemoryPropertyFlags type);
    CELL_FUNCTION_INTERNAL uint32_t GetMemoryTypeIndex(VkImage image, const VkMemoryPropertyFlags type);

    CELL_FUNCTION Result CreateDevice(const char* CELL_NONNULL* CELL_NONNULL extensions, const uint32_t count);

    // UPPER BITS: graphics LOWER BITS: transfer
    CELL_FUNCTION static uint64_t QueryPhysicalDeviceQueues(VkPhysicalDevice CELL_NONNULL device);

    CELL_FUNCTION Result CreateImageView(
        VkImageView& view,
        VkImage CELL_NONNULL image,
        const VkFormat format,
        const VkImageViewType type = VK_IMAGE_VIEW_TYPE_2D,
        const VkImageAspectFlagBits aspectMask = VK_IMAGE_ASPECT_COLOR_BIT
    );

    VkInstance instance;
    PFN_vkCmdBeginRenderingKHR beginRendering;
    PFN_vkCmdEndRenderingKHR endRendering;

    VkPhysicalDevice physicalDevice = nullptr;
    uint32_t physicalDeviceQueueGraphics = (uint32_t)-1;
    uint32_t physicalDeviceQueueTransfer = (uint32_t)-1;

    VkPhysicalDeviceMemoryProperties physicalDeviceProperties = {
        .memoryTypeCount = 0,
        .memoryTypes = { },
        .memoryHeapCount = 0,
        .memoryHeaps = { }
    };

    VkDevice device = nullptr;
    VkQueue deviceQueueGraphics = nullptr;
    VkQueue deviceQueueTransfer = nullptr;
};

}

}
