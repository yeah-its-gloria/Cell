// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Internal.hh"

#include <Cell/System/Platform/Linux.hh>

#include <vulkan/vulkan_wayland.h>

using namespace Cell;
using namespace Cell::System;
using namespace Cell::Vulkan;

CELL_FUNCTION_INTERNAL Vulkan::Result createPlatformSurface(VkSurfaceKHR* surface, VkInstance instance, IPlatform& platform) {
    Platform::Linux* _linux = (Platform::Linux*)&platform;

    if (_linux->GetWaylandDisplay() == nullptr || _linux->GetWaylandSurface() == nullptr) {
        return Vulkan::Result::PlatformNotReadyYet;
    }

    // Surface creation

    const VkWaylandSurfaceCreateInfoKHR surfaceInfo = {
        .sType     = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR,
        .pNext     = nullptr,
        .flags     = 0,

        .display   = _linux->GetWaylandDisplay(),
        .surface   = _linux->GetWaylandSurface()
    };

    const VkResult vk_result = vkCreateWaylandSurfaceKHR(instance, &surfaceInfo, nullptr, surface);
    switch (vk_result) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        return Vulkan::Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        return Vulkan::Result::OutOfDeviceMemory;
    }

    default: {
        System::Panic("vkCreateWaylandSurfaceKHR failed");
    }
    }

    return Vulkan::Result::Success;
}
