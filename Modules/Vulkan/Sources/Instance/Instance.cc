// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/Instance.hh>

#ifdef CELL_PLATFORM_WINDOWS
#include <Cell/System/Platform/Windows/Includes.h>
#include <vulkan/vulkan_win32.h>
#elif CELL_PLATFORM_MACOS
#include <vulkan/vulkan_macos.h>
#elif CELL_PLATFORM_LINUX
#include <vulkan/vulkan_wayland.h>
#else
#error No surface implementation available for the current platform.
#endif

namespace Cell::Vulkan {

Wrapped<Instance*, Result> Instance::New() {
    const char* extensions[] = {
        VK_KHR_SURFACE_EXTENSION_NAME,

#ifdef CELL_PLATFORM_WINDOWS
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#elif CELL_PLATFORM_MACOS
        VK_MVK_MACOS_SURFACE_EXTENSION_NAME,
        VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME
#elif CELL_PLATFORM_LINUX
        VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME
#endif
    };

    return Instance::New(extensions, sizeof(extensions) / sizeof(char*));
}

Wrapped<Instance*, Result> Instance::New(const char** extensions, const uint32_t count) {
    const VkApplicationInfo applicationInfo = {
        .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext              = nullptr,

        .pApplicationName   = "Cell",
        .applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),

        .pEngineName        = "CellVulkan",
        .engineVersion      = VK_MAKE_API_VERSION(0, 1, 0, 0),

        .apiVersion         = VK_API_VERSION_1_2
    };

    const VkInstanceCreateInfo instanceInfo = {
        .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext                   = nullptr,
#ifdef CELL_PLATFORM_MACOS
        .flags                   = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
#else
        .flags                   = 0,
#endif

        .pApplicationInfo        = &applicationInfo,

        .enabledLayerCount       = 0,
        .ppEnabledLayerNames     = nullptr,

        .enabledExtensionCount   = count,
        .ppEnabledExtensionNames = extensions
    };

    VkInstance inst = nullptr;
    const VkResult result = vkCreateInstance(&instanceInfo, nullptr, &inst);
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

    const PFN_vkCmdBeginRenderingKHR beginRendering = (PFN_vkCmdBeginRenderingKHR)vkGetInstanceProcAddr(inst, "vkCmdBeginRenderingKHR");
    CELL_ASSERT(beginRendering != nullptr);

    const PFN_vkCmdEndRenderingKHR endRendering = (PFN_vkCmdEndRenderingKHR)vkGetInstanceProcAddr(inst, "vkCmdEndRenderingKHR");
    CELL_ASSERT(endRendering != nullptr);

    const PFN_vkCmdSetCullModeEXT setCullMode = (PFN_vkCmdSetCullModeEXT)vkGetInstanceProcAddr(inst, "vkCmdSetCullModeEXT");
    CELL_ASSERT(endRendering != nullptr);

    return new Instance(inst, beginRendering, endRendering, setCullMode);
}

Instance::~Instance() {
    vkDestroyInstance(this->instance, nullptr);
}

}
