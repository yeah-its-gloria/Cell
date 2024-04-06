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

class Device : public NoCopyObject {
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

    // Creates a WSI implementation render target backed by the given shell.
    // Backed by 4 images by default.
    CELL_FUNCTION Wrapped<class WSITarget*, Result> CreateWSITarget(Shell::IShell* CELL_NONNULL shell, const uint8_t depth = 4);

private:
    CELL_FUNCTION_INTERNAL Device(VkPhysicalDevice pd, uint32_t pgi,
                                  uint32_t pti, VkPhysicalDeviceMemoryProperties pr,
                                  VkDevice d, VkQueue qg,
                                  VkQueue qt, VkCommandPool pg,
                                  VkCommandPool pt, Instance* i)
        : physicalDevice(pd), physicalDeviceQueueGraphics(pgi),
          physicalDeviceQueueTransfer(pti), physicalDeviceProperties(pr),
          device(d), deviceQueueGraphics(qg),
          deviceQueueTransfer(qt), graphicsPool(pg),
          transferPool(pt), instance(i) { }

    struct SurfaceStaticInfo {
        const VkSurfaceFormatKHR format;
        const VkPresentModeKHR mode;
    };

    struct SurfaceDynamicInfo {
        const VkSurfaceCapabilitiesKHR capabilities;
        const VkExtent2D extent;
    };

    struct SwapchainInfo {
        VkSurfaceKHR& surface;
        const VkSurfaceFormatKHR& format;
        const VkPresentModeKHR& mode;
        const VkSurfaceCapabilitiesKHR& capabilities;
        const VkExtent2D& extent;
        const uint8_t& depth;
    };

    struct SwapchainData {
        VkSwapchainKHR swapchain;
        Collection::List<VkImage> images;
        Collection::List<VkImageView> views;
    };

    CELL_FUNCTION Result CreateImageView(
        VkImageView& view,
        VkImage CELL_NONNULL image,
        const VkFormat format,
        const VkImageViewType type = VK_IMAGE_VIEW_TYPE_2D,
        const VkImageAspectFlagBits aspectMask = VK_IMAGE_ASPECT_COLOR_BIT
    );

    CELL_FUNCTION_TEMPLATE uint32_t GetMemoryTypeIndex(const uint32_t bits, const VkMemoryPropertyFlags type) {
        uint32_t typeIndex = (uint32_t)-1;
        for (uint32_t i = 0; i < this->physicalDeviceProperties.memoryTypeCount; i++) {
            if (bits & (1 << i) && this->physicalDeviceProperties.memoryTypes[i].propertyFlags & type) {
                typeIndex = i;
                break;
            }
        }

        CELL_ASSERT(typeIndex != (uint32_t)-1);

        return typeIndex;
    }

    CELL_FUNCTION_INTERNAL Wrapped<SurfaceStaticInfo, Result> GetSurfaceStatics(VkSurfaceKHR& surface);
    CELL_FUNCTION_INTERNAL Wrapped<SurfaceDynamicInfo, Result> GetSurfaceDynamics(VkSurfaceKHR& surface, const Shell::Extent& shellExtent);
    CELL_FUNCTION_INTERNAL Wrapped<SwapchainData, Result> CreateSwapchainForSurface(const SwapchainInfo& info);
    CELL_FUNCTION_INTERNAL Wrapped<Image*, Result> CreateDepthStencilImage(const VkExtent2D& info);

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
