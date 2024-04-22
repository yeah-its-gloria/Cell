// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Utilities/MinMaxClamp.hh>
#include <Cell/Renderer/Vulkan/Device.hh>

namespace Cell::Renderer::Vulkan {

Wrapped<Device::SurfaceStaticInfo, Result> Device::GetSurfaceStatics(VkSurfaceKHR& surface) {
    VkBool32 presentable = VK_FALSE;
    VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(this->physicalDevice, this->physicalDeviceQueueGraphics, surface, &presentable);
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

    case VK_ERROR_SURFACE_LOST_KHR: {
        return Result::SurfaceLost;
    }

    default: {
        System::Panic("vkGetPhysicalDeviceSurfaceSupportKHR failed");
    }
    }

    if (presentable == VK_FALSE) {
        return Result::Unsupported;
    }

    // Format retrieval

    uint32_t formatCount = 0;
    result = vkGetPhysicalDeviceSurfaceFormatsKHR(this->physicalDevice, surface, &formatCount, nullptr);
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

    case VK_ERROR_SURFACE_LOST_KHR: {
        return Result::SurfaceLost;
    }

    default: {
        System::Panic("vkGetPhysicalDeviceSurfaceFormatsKHR failed");
    }
    }

    CELL_ASSERT(formatCount > 0);

    Collection::List<VkSurfaceFormatKHR> formats(formatCount);
    result = vkGetPhysicalDeviceSurfaceFormatsKHR(this->physicalDevice, surface, &formatCount, formats.AsRaw());
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

    case VK_ERROR_SURFACE_LOST_KHR: {
        return Result::SurfaceLost;
    }

    default: {
        System::Panic("vkGetPhysicalDeviceSurfaceFormatsKHR failed");
    }
    }

    uint32_t modeCount = 0;
    result = vkGetPhysicalDeviceSurfacePresentModesKHR(this->physicalDevice, surface, &modeCount, nullptr);
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

    case VK_ERROR_SURFACE_LOST_KHR: {
        return Result::SurfaceLost;
    }

    default: {
        System::Panic("vkGetPhysicalDeviceSurfacePresentModesKHR failed");
    }
    }

    CELL_ASSERT(modeCount > 0);

    Collection::List<VkPresentModeKHR> modes(modeCount);
    result = vkGetPhysicalDeviceSurfacePresentModesKHR(this->physicalDevice, surface, &modeCount, modes.AsRaw());
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

    case VK_ERROR_SURFACE_LOST_KHR: {
        return Result::SurfaceLost;
    }

    default: {
        System::Panic("vkGetPhysicalDeviceSurfacePresentModesKHR failed");
    }
    }

    const VkSurfaceFormatKHR format = [&formats]{
        for (VkSurfaceFormatKHR format : formats) {
            if (format.format == VK_FORMAT_R8G8B8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return format;
            }
        }

        return formats[0];
    }();

    const VkPresentModeKHR mode = [&modes]{
        for (VkPresentModeKHR mode : modes) {
            if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return VK_PRESENT_MODE_MAILBOX_KHR;
            }
        }

        return VK_PRESENT_MODE_IMMEDIATE_KHR;
    }();

    return SurfaceStaticInfo { format, mode };
}

Wrapped<Device::SurfaceDynamicInfo, Result> Device::GetSurfaceDynamics(VkSurfaceKHR& surface, const Shell::Extent& shellExtent) {
    VkSurfaceCapabilitiesKHR capabilities = {
        .minImageCount = 0,
        .maxImageCount = 0,
        .currentExtent = { 0, 0 },
        .minImageExtent = { 0, 0 },
        .maxImageExtent = { 0, 0 },
        .maxImageArrayLayers = 0,
        .supportedTransforms = 0,
        .currentTransform = (VkSurfaceTransformFlagBitsKHR)0,
        .supportedCompositeAlpha = 0,
        .supportedUsageFlags = 0
    };

    const VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(this->physicalDevice, surface, &capabilities);
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

    case VK_ERROR_SURFACE_LOST_KHR: {
        return Result::SurfaceLost;
    }

    default: {
        System::Panic("vkGetPhysicalDeviceSurfaceCapabilitiesKHR failed");
    }
    }

    if (capabilities.currentExtent.width != UINT32_MAX && capabilities.currentExtent.height != UINT32_MAX) {
        return Device::SurfaceDynamicInfo { capabilities, capabilities.currentExtent };
    }

    const VkExtent2D extent = {
        .width  = Utilities::Clamp(shellExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
        .height = Utilities::Clamp(shellExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height),
    };

    return Device::SurfaceDynamicInfo { capabilities, extent };
}

}
