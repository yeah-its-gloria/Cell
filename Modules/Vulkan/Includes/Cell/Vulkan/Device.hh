// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Shell/Shell.hh>
#include <Cell/Vulkan/RenderTarget.hh>

#if CELL_MODULES_OPENXR_AVAILABLE
namespace Cell::OpenXR { class Session; }
#endif

namespace Cell::Vulkan {

enum class QueueType : uint8_t {
    Graphics,
    Transfer
};

enum class Stage : uint8_t {
    Vertex,
    Fragment
};

class Device : public Object {
friend Instance;

friend class Buffer;
friend class CommandBuffer;
friend class Image;
friend class Pipeline;
friend class WSITarget;

#if CELL_MODULES_OPENXR_AVAILABLE
friend OpenXR::Instance;
friend OpenXR::Session;
#endif

public:
    // Destructs the device, alongside all resources created with it.
    CELL_FUNCTION ~Device();

    // Creates a data buffer resource.
    CELL_FUNCTION Wrapped<class Buffer*, Result> CreateBuffer(const size_t size,
                                                              const VkBufferUsageFlags usage,
                                                              const VkMemoryPropertyFlags memoryType,
                                                              const VkSharingMode shareMode = VK_SHARING_MODE_EXCLUSIVE
    );

    // Creates an image resource.
    CELL_FUNCTION Wrapped<class Image*, Result> CreateImage(const uint32_t width,
                                                            const uint32_t height,
                                                            const VkFormat format = VK_FORMAT_R8G8B8A8_SRGB,
                                                            const VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL,
                                                            const VkImageAspectFlags viewAspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                                            const VkImageUsageFlags usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT
    );

    // Creates a command buffer.
    CELL_FUNCTION Wrapped<class CommandBuffer*, Result> CreateCommandBuffer(const QueueType queue = QueueType::Graphics);

    // Creates a pipeline.
    CELL_FUNCTION Wrapped<class Pipeline*, Result> CreatePipeline(IRenderTarget* CELL_NONNULL target);

    // Rendering utility - Renders to the next swapchain image index with the given buffer.
    CELL_FUNCTION Result RenderImage(IRenderTarget* CELL_NONNULL target, class CommandBuffer* CELL_NONNULL buffer);

    // Creates a WSI render target.
    CELL_FUNCTION Wrapped<class WSITarget*, Result> CreateWSITarget(Shell::IShell* CELL_NONNULL shell);

    CELL_NON_COPYABLE(Device)

private:
    CELL_FUNCTION_INTERNAL CELL_INLINE Device(VkPhysicalDevice pd, uint32_t pgi,
                                              uint32_t pti, VkPhysicalDeviceMemoryProperties pr,
                                              VkDevice d, VkQueue qg,
                                              VkQueue qt, VkCommandPool pg,
                                              VkCommandPool pt, Instance* i)
        : physicalDevice(pd), physicalDeviceQueueGraphics(pgi),
          physicalDeviceQueueTransfer(pti), physicalDeviceProperties(pr),
          device(d), deviceQueueGraphics(qg),
          deviceQueueTransfer(qt), graphicsPool(pg),
          transferPool(pt), instance(i) { }

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

    VkCommandPool graphicsPool;
    VkCommandPool transferPool;

    Instance* instance;
};

}
