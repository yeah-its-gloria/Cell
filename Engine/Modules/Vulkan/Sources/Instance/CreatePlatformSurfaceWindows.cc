// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Internal.hh"

#include <Cell/System/Platform/Windows.hh>

#include <vulkan/vulkan_win32.h>

using namespace Cell;
using namespace Cell::Vulkan;

CELL_FUNCTION_INTERNAL Result createPlatformSurface(VkSurfaceKHR* surface, VkInstance instance, System::IPlatform& platform) {
    System::Platform::Windows* windows = (System::Platform::Windows*)&platform;
    if (windows->GetWindow() == nullptr) {
        return Result::PlatformNotReadyYet;
    }

    // Surface creation

    const VkWin32SurfaceCreateInfoKHR surfaceInfo = {
        .sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .pNext     = nullptr,
        .flags     = 0,

        .hinstance = windows->GetInstance(),
        .hwnd      = windows->GetWindow()
    };

    VkResult vk_result = vkCreateWin32SurfaceKHR(instance, &surfaceInfo, nullptr, surface);
    switch (vk_result) {
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
