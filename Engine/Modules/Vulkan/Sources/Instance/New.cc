// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/Instance.hh>

#ifdef CELL_PLATFORM_WINDOWS

#include <Cell/System/Platform/Windows/Includes.h>

#include <vulkan/vulkan_win32.h>

#elif CELL_PLATFORM_LINUX
#include <vulkan/vulkan_wayland.h>
#else
#error No surface implementation available for the current platform.
#endif

namespace Cell::Vulkan {

Wrapped<Instance*, Result> Instance::New() {
    const char* extensions[2] = {
        VK_KHR_SURFACE_EXTENSION_NAME,

#ifdef CELL_PLATFORM_WINDOWS
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#elif CELL_PLATFORM_LINUX
        VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME
#endif
    };

    return Instance::New(extensions, 2);
}

Wrapped<Instance*, Result> Instance::New(const char** extensions, const uint32_t count) {
    const VkApplicationInfo applicationInfo = {
        .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext              = nullptr,

        .pApplicationName   = "Cell",
        .applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),

        .pEngineName        = "Cell",
        .engineVersion      = VK_MAKE_API_VERSION(0, 1, 0, 0),

        .apiVersion         = VK_API_VERSION_1_2
    };

    const VkInstanceCreateInfo instanceInfo = {
        .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext                   = nullptr,
        .flags                   = 0,

        .pApplicationInfo        = &applicationInfo,

        .enabledLayerCount       = 0,
        .ppEnabledLayerNames     = nullptr,

        .enabledExtensionCount   = count,
        .ppEnabledExtensionNames = extensions
    };

    VkInstance vkInstance = nullptr;
    const VkResult result = vkCreateInstance(&instanceInfo, nullptr, &vkInstance);
    switch (result) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        return Result::OutOfDeviceMemory;
    }

    case VK_ERROR_EXTENSION_NOT_PRESENT: {
        return Result::FeatureMissing;
    }

    case VK_ERROR_LAYER_NOT_PRESENT: {
        return Result::DebugFeatureMissing;
    }

    case VK_ERROR_INCOMPATIBLE_DRIVER: {
        return Result::RunningOnIncompatibleHardware;
    }

    default: {
        System::Panic("vkCreateInstance failed");
    }
    }

    PFN_vkCmdBeginRenderingKHR beginRendering = (PFN_vkCmdBeginRenderingKHR)vkGetInstanceProcAddr(vkInstance, "vkCmdBeginRenderingKHR");
    CELL_ASSERT(beginRendering != nullptr);

    PFN_vkCmdEndRenderingKHR endRendering = (PFN_vkCmdEndRenderingKHR)vkGetInstanceProcAddr(vkInstance, "vkCmdEndRenderingKHR");
    CELL_ASSERT(endRendering != nullptr);

    PFN_vkCmdSetCullModeEXT setCullMode = (PFN_vkCmdSetCullModeEXT)vkGetInstanceProcAddr(vkInstance, "vkCmdSetCullModeEXT");
    CELL_ASSERT(endRendering != nullptr);

    return new Instance(vkInstance, beginRendering, endRendering, setCullMode);
}

}
