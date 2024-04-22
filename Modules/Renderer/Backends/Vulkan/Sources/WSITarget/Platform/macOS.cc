// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "../Platform.hh"

#include <Cell/Shell/Implementations/macOS.hh>
#include <vulkan/vulkan_macos.h>

namespace Cell::Renderer::Vulkan {

CELL_FUNCTION_INTERNAL Result createPlatformSurface(VkSurfaceKHR* surface, VkInstance instance, Shell::IShell* platform) {
    Shell::Implementations::macOS* macos = (Shell::Implementations::macOS*)platform;

    const VkMacOSSurfaceCreateInfoMVK surfaceInfo = {
        .sType     = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK,
        .pNext     = nullptr,
        .flags     = 0,

        .pView     = macos->GetMetalLayer()
    };

    const VkResult vkResult = vkCreateMacOSSurfaceMVK(instance, &surfaceInfo, nullptr, surface);
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
        System::Panic("vkCreateMacOSSurfaceMVK failed");
    }
    }

    return Result::Success;
}

}
