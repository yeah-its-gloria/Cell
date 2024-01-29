// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "../Platform.hh"

#include <Cell/Shell/Implementations/Linux.hh>
#include <vulkan/vulkan_wayland.h>

namespace Cell::Vulkan {

CELL_FUNCTION_INTERNAL Result createPlatformSurface(VkSurfaceKHR* surface, VkInstance instance, Shell::IShell* platform) {
    Shell::Implementations::Linux* _linux = (Shell::Implementations::Linux*)platform;

    const VkWaylandSurfaceCreateInfoKHR surfaceInfo = {
        .sType     = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR,
        .pNext     = nullptr,
        .flags     = 0,

        .display   = _linux->GetWaylandDisplay(),
        .surface   = _linux->GetWaylandSurface()
    };

    const VkResult vkResult = vkCreateWaylandSurfaceKHR(instance, &surfaceInfo, nullptr, surface);
    switch (vkResult) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        return Result::OutOfDeviceMemory;
    }

    default: {
        System::Panic("vkCreateWaylandSurfaceKHR failed");
    }
    }

    return Result::Success;
}

}
