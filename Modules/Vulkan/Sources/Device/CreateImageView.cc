// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/Device.hh>

namespace Cell::Vulkan {

Result Device::CreateImageView(VkImageView& view, VkImage image, const VkFormat format, const VkImageViewType type, const VkImageAspectFlagBits aspectMask) {
    if (type > VK_IMAGE_VIEW_TYPE_CUBE_ARRAY) {
        return Result::InvalidParameters;
    }

    const VkImageViewCreateInfo viewInfo = {
        .sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext                           = nullptr,
        .flags                           = 0,

        .image                           = image,
        .viewType                        = type,
        .format                          = format,

        .components.r                    = VK_COMPONENT_SWIZZLE_R,
        .components.g                    = VK_COMPONENT_SWIZZLE_G,
        .components.b                    = VK_COMPONENT_SWIZZLE_B,
        .components.a                    = VK_COMPONENT_SWIZZLE_A,

        .subresourceRange.aspectMask     = (const VkImageAspectFlags)aspectMask,
        .subresourceRange.baseMipLevel   = 0,
        .subresourceRange.levelCount     = 1,
        .subresourceRange.baseArrayLayer = 0,
        .subresourceRange.layerCount     = 1
    };

    const VkResult result = vkCreateImageView(this->device, &viewInfo, nullptr, &view);
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

    default: {
        System::Panic("vkCreateImageView failed");
    }
    }

    return Result::Success;
}

}
