// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Utilities/MinMaxClamp.hh>
#include <Cell/Vulkan/WSITarget.hh>

namespace Cell::Vulkan {

Result WSITarget::RetrieveProperties() {
    const VkResult vkResult = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(this->instance->physicalDevice, this->surface, &this->capabilities);
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

    case VK_ERROR_SURFACE_LOST_KHR: {
        return Result::SurfaceLost;
    }

    default: {
        System::Panic("vkGetPhysicalDeviceSurfaceCapabilitiesKHR failed");
    }
    }

    if (this->capabilities.currentExtent.width != UINT32_MAX && this->capabilities.currentExtent.height != UINT32_MAX) {
        this->extent = this->capabilities.currentExtent;
        return Result::Success;
    }

    const Shell::Extent extent = this->shell->GetDrawableExtent().Unwrap();

    this->extent.width = Utilities::Clamp(extent.width, this->capabilities.minImageExtent.width, this->capabilities.maxImageExtent.width);
    this->extent.height = Utilities::Clamp(extent.height, this->capabilities.minImageExtent.height, this->capabilities.maxImageExtent.height);

    return Result::Success;
}

}
