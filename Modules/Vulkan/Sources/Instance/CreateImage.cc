// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/Image.hh>

namespace Cell::Vulkan {

Wrapped<Image*, Result>
Instance::CreateImage(const uint32_t width, const uint32_t height, const VkFormat format, const VkImageTiling tiling, const VkImageAspectFlags viewAspectMask, const VkImageUsageFlags usage) {
    if (this->device == nullptr) {
        return Result::InvalidState;
    }

    if (width == 0 || height == 0) {
        return Result::InvalidParameters;
    }

    const VkImageCreateInfo imageInfo = {
        .sType               = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .pNext               = nullptr,
        .flags               = 0,

        .imageType           = VK_IMAGE_TYPE_2D,
        .format              = format,

        .extent.width        = width,
        .extent.height       = height,
        .extent.depth        = 1,

        .mipLevels           = 1,
        .arrayLayers         = 1,
        .samples             = VK_SAMPLE_COUNT_1_BIT,

        .tiling              = tiling,
        .usage               = (VkImageUsageFlags)usage,
        .sharingMode         = VK_SHARING_MODE_EXCLUSIVE,

        .pQueueFamilyIndices = nullptr,
        .initialLayout       = VK_IMAGE_LAYOUT_UNDEFINED
    };

    VkImage vkImage = nullptr;
    VkResult vkResult = vkCreateImage(this->device, &imageInfo, nullptr, &vkImage);
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
        System::Panic("vkCreateImage failed");
    }
    }

    VkMemoryRequirements requirements;
    vkGetImageMemoryRequirements(device, vkImage, &requirements);

    const VkMemoryAllocateInfo allocInfo = {
        .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext           = nullptr,

        .allocationSize  = requirements.size,
        .memoryTypeIndex = this->GetMemoryTypeIndex(vkImage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
    };

    VkDeviceMemory memory = nullptr;
    vkResult = vkAllocateMemory(device, &allocInfo, nullptr, &memory);
    switch (vkResult) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        vkDestroyImage(this->device, vkImage, nullptr);
        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        vkDestroyImage(this->device, vkImage, nullptr);
        return Result::OutOfDeviceMemory;
    }

    default: {
        System::Panic("vkAllocateMemory failed");
    }
    }

    vkResult = vkBindImageMemory(device, vkImage, memory, 0);
    switch (vkResult) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        vkDestroyImage(this->device, vkImage, nullptr);
        vkFreeMemory(this->device, memory, nullptr);
        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        vkDestroyImage(this->device, vkImage, nullptr);
        vkFreeMemory(this->device, memory, nullptr);
        return Result::OutOfDeviceMemory;
    }

    default: {
        System::Panic("vkBindImageMemory failed");
    }
    }

    VkImageView view = nullptr;
    Result result = this->CreateImageView(view, vkImage, format, VK_IMAGE_VIEW_TYPE_2D, (VkImageAspectFlagBits)viewAspectMask);
    if (result != Result::Success) {
        vkDestroyImage(this->device, vkImage, nullptr);
        vkFreeMemory(this->device, memory, nullptr);
        return result;
    }

    const VkSamplerCreateInfo samplerInfo = {
        .sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .pNext                   = nullptr,
        .flags                   = 0,

        .magFilter               = VK_FILTER_LINEAR,
        .minFilter               = VK_FILTER_LINEAR,
        .mipmapMode              = VK_SAMPLER_MIPMAP_MODE_NEAREST,

        .addressModeU            = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .addressModeV            = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .addressModeW            = VK_SAMPLER_ADDRESS_MODE_REPEAT,

        .mipLodBias              = 0.f,

        .anisotropyEnable        = VK_TRUE,
        .maxAnisotropy           = 2,

        .compareEnable           = VK_FALSE,
        .compareOp               = VK_COMPARE_OP_ALWAYS,

        .minLod                  = 0.f,
        .maxLod                  = 0.f,

        .borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK,

        .unnormalizedCoordinates = VK_FALSE
    };

    VkSampler sampler = nullptr;
    vkResult = vkCreateSampler(this->device, &samplerInfo, nullptr, &sampler);
    switch (vkResult) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        vkDestroyImageView(this->device, view, nullptr);
        vkDestroyImage(this->device, vkImage, nullptr);
        vkFreeMemory(this->device, memory, nullptr);
        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        vkDestroyImageView(this->device, view, nullptr);
        vkDestroyImage(this->device, vkImage, nullptr);
        vkFreeMemory(this->device, memory, nullptr);
        return Result::OutOfDeviceMemory;
    }

    default: {
        System::Panic("vkCreateSampler failed");
    }
    }

    return new Image(this, vkImage, memory, view, sampler, width, height, 1);
}

}
