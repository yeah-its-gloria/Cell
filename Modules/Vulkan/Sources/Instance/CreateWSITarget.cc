// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Internal.hh"

#include <Cell/System/BlockImpl.hh>
#include <Cell/Vulkan/WSITarget.hh>

using namespace Cell;

CELL_INLINE VkSurfaceFormatKHR selectFormat(const VkSurfaceFormatKHR* CELL_NONNULL formats, const uint32_t count) {
    for (uint32_t index = 0; index < count; index++) {
        if (formats[index].format == VK_FORMAT_R8G8B8A8_SRGB && formats[index].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return formats[index];
        }
    }

    return formats[0];
}

CELL_INLINE VkPresentModeKHR selectPresentMode(const VkPresentModeKHR* CELL_NONNULL modes, const uint32_t count) {
    for (uint32_t index = 0; index < count; index++) {
        if (modes[index] == VK_PRESENT_MODE_MAILBOX_KHR) {
            return VK_PRESENT_MODE_MAILBOX_KHR;
        }
    }

    return VK_PRESENT_MODE_IMMEDIATE_KHR;
}

namespace Cell::Vulkan {

Wrapped<WSITarget*, Result> Instance::CreateWSITarget(Shell::IShell* shell) {
    if (this->device == nullptr) {
        return Result::InvalidState;
    }

    VkSurfaceKHR surface = nullptr;
    const Result result = createPlatformSurface(&surface, this->instance, shell);
    if (result != Result::Success) {
        return result;
    }

    // Presentation verification

    VkBool32 canPresent = VK_FALSE;
    VkResult vkResult = vkGetPhysicalDeviceSurfaceSupportKHR(this->physicalDevice, this->physicalDeviceQueueGraphics, surface, &canPresent);
    switch (vkResult) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        vkDestroySurfaceKHR(this->instance, surface, nullptr);
        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        vkDestroySurfaceKHR(this->instance, surface, nullptr);
        return Result::OutOfDeviceMemory;
    }

    case VK_ERROR_SURFACE_LOST_KHR: {
        vkDestroySurfaceKHR(this->instance, surface, nullptr);
        return Result::SurfaceLost;
    }

    default: {
        System::Panic("vkGetPhysicalDeviceSurfaceSupportKHR failed");
    }
    }

    if (canPresent == VK_FALSE) {
        vkDestroySurfaceKHR(this->instance, surface, nullptr);
        return Result::RunningOnIncompatibleHardware;
    }

    // Format retrieval

    uint32_t formatCount = 0;
    vkResult = vkGetPhysicalDeviceSurfaceFormatsKHR(this->physicalDevice, surface, &formatCount, nullptr);
    switch (vkResult) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        vkDestroySurfaceKHR(this->instance, surface, nullptr);
        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        vkDestroySurfaceKHR(this->instance, surface, nullptr);
        return Result::OutOfDeviceMemory;
    }

    case VK_ERROR_SURFACE_LOST_KHR: {
        vkDestroySurfaceKHR(this->instance, surface, nullptr);
        return Result::SurfaceLost;
    }

    default: {
        System::Panic("vkGetPhysicalDeviceSurfaceFormatsKHR failed");
    }
    }

    CELL_ASSERT(formatCount > 0);

    System::OwnedBlock<VkSurfaceFormatKHR> formats(formatCount);
    vkResult = vkGetPhysicalDeviceSurfaceFormatsKHR(this->physicalDevice, surface, &formatCount, formats);
    switch (vkResult) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        vkDestroySurfaceKHR(this->instance, surface, nullptr);
        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        vkDestroySurfaceKHR(this->instance, surface, nullptr);
        return Result::OutOfDeviceMemory;
    }

    case VK_ERROR_SURFACE_LOST_KHR: {
        vkDestroySurfaceKHR(this->instance, surface, nullptr);
        return Result::SurfaceLost;
    }

    default: {
        System::Panic("vkGetPhysicalDeviceSurfaceFormatsKHR failed");
    }
    }

    // Presentation mode retrieval

    uint32_t presentationModeCount = 0;
    vkResult = vkGetPhysicalDeviceSurfacePresentModesKHR(this->physicalDevice, surface, &presentationModeCount, nullptr);
    switch (vkResult) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        vkDestroySurfaceKHR(this->instance, surface, nullptr);
        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        vkDestroySurfaceKHR(this->instance, surface, nullptr);
        return Result::OutOfDeviceMemory;
    }

    case VK_ERROR_SURFACE_LOST_KHR: {
        vkDestroySurfaceKHR(this->instance, surface, nullptr);
        return Result::SurfaceLost;
    }

    default: {
        System::Panic("vkGetPhysicalDeviceSurfacePresentModesKHR failed");
    }
    }

    CELL_ASSERT(presentationModeCount > 0);

    System::OwnedBlock<VkPresentModeKHR> presentationModes(presentationModeCount);
    vkResult = vkGetPhysicalDeviceSurfacePresentModesKHR(this->physicalDevice, surface, &presentationModeCount, presentationModes);
    switch (vkResult) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        vkDestroySurfaceKHR(this->instance, surface, nullptr);
        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        vkDestroySurfaceKHR(this->instance, surface, nullptr);
        return Result::OutOfDeviceMemory;
    }

    case VK_ERROR_SURFACE_LOST_KHR: {
        vkDestroySurfaceKHR(this->instance, surface, nullptr);
        return Result::SurfaceLost;
    }

    default: {
        System::Panic("vkGetPhysicalDeviceSurfacePresentModesKHR failed");
    }
    }

    return new WSITarget(this, shell, surface, selectFormat(formats, formatCount), selectPresentMode(presentationModes, presentationModeCount));
}

}
