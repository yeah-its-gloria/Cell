// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "../Platform.hh"

#include <Cell/Shell/Implementations/Windows.hh>
#include <vulkan/vulkan_win32.h>

namespace Cell::Renderer::Vulkan {

Result createPlatformSurface(VkSurfaceKHR* surface, VkInstance instance, Shell::IShell* platform) {
    Shell::Implementations::Windows* windows = (Shell::Implementations::Windows*)platform;

    const VkWin32SurfaceCreateInfoKHR surfaceInfo = {
        .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,

        .hinstance = windows->GetInstance(),
        .hwnd = windows->GetWindow()
    };

    const VkResult vkResult = vkCreateWin32SurfaceKHR(instance, &surfaceInfo, nullptr, surface);
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
        System::Panic("vkCreateWin32SurfaceKHR failed");
    }
    }

    return Result::Success;
}

}
