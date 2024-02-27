// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Wrapped.hh>
#include <Cell/Vulkan/Result.hh>

#include <vulkan/vulkan.h>

#if CELL_MODULES_OPENXR_AVAILABLE
namespace Cell::OpenXR { class Instance; }
#endif

namespace Cell::Vulkan {

class Instance : public Object {
friend class CommandBuffer;
friend class Device;
friend class WSITarget;

#if CELL_MODULES_OPENXR_AVAILABLE
friend OpenXR::Instance;
#endif

public:
    // Creates a new instance and initializes a Vulkan instance for it.
    CELL_FUNCTION static Wrapped<Instance*, Result> New();

    // Creates a new instance and initializes a Vulkan instance for it, with the given extensions.
    CELL_FUNCTION static Wrapped<Instance*, Result> New(const char* CELL_NONNULL * CELL_NONNULL extensions, const uint32_t count);

    // Destructor for instances.
    CELL_FUNCTION ~Instance();

    // Initializes the best available device.
    CELL_FUNCTION Wrapped<class Device*, Result> CreateDevice();

    // Initializes the best available device.
    // Allows supplying an existing physical device.
    CELL_FUNCTION Wrapped<class Device*, Result> CreateDevice(const char* CELL_NONNULL * CELL_NONNULL extensions,
                                                              const uint32_t count,
                                                              VkPhysicalDevice CELL_NULLABLE physicalDevice = nullptr);

    CELL_NON_COPYABLE(Instance)

private:
    CELL_INLINE Instance(VkInstance CELL_NONNULL instance,
                         PFN_vkCmdBeginRenderingKHR CELL_NONNULL beginRendering,
                         PFN_vkCmdEndRenderingKHR CELL_NONNULL endRendering,
                         PFN_vkCmdSetCullModeEXT CELL_NONNULL setCullMode)
        : instance(instance), beginRendering(beginRendering), endRendering(endRendering), setCullMode(setCullMode) { }

    struct PhysicalDeviceQueues {
        uint32_t graphics;
        uint32_t transfer;
    };

    struct QueryPhysicalDeviceResult {
        VkPhysicalDevice device;
        VkPhysicalDeviceMemoryProperties properties;
        PhysicalDeviceQueues queues;
    };

    CELL_FUNCTION_INTERNAL Wrapped<QueryPhysicalDeviceResult, Result> QueryPhysicalDevice(VkPhysicalDevice CELL_NULLABLE device);

    CELL_FUNCTION_INTERNAL static uint16_t ScorePhysicalDevice(VkPhysicalDevice CELL_NONNULL device);
    CELL_FUNCTION_INTERNAL static PhysicalDeviceQueues QueryPhysicalDeviceQueues(VkPhysicalDevice CELL_NONNULL device);

    VkInstance instance;

    PFN_vkCmdBeginRenderingKHR beginRendering;
    PFN_vkCmdEndRenderingKHR endRendering;
    PFN_vkCmdSetCullModeEXT setCullMode;
};

}
