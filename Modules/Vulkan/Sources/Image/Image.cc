// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/Image.hh>

namespace Cell::Vulkan {

Wrapped<Image*, Result> Device::CreateImage(const uint32_t width, const uint32_t height, const VkFormat format,
                                            const VkImageTiling tiling, const VkImageAspectFlags viewAspectMask, const VkImageUsageFlags usage) {
    if (width == 0 || height == 0) {
        return Result::InvalidParameters;
    }

    const VkImageCreateInfo imageInfo = {
        .sType                 = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .pNext                 = nullptr,
        .flags                 = 0,

        .imageType             = VK_IMAGE_TYPE_2D,
        .format                = format,

        .extent.width          = width,
        .extent.height         = height,
        .extent.depth          = 1,

        .mipLevels             = 1,
        .arrayLayers           = 1,
        .samples               = VK_SAMPLE_COUNT_1_BIT,

        .tiling                = tiling,
        .usage                 = (VkImageUsageFlags)usage,
        .sharingMode           = VK_SHARING_MODE_EXCLUSIVE,

        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices   = nullptr,

        .initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED
    };

    VkImage image = nullptr;
    VkResult result = vkCreateImage(this->device, &imageInfo, nullptr, &image);
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
        System::Panic("vkCreateImage failed");
    }
    }

    VkMemoryRequirements requirements;
    vkGetImageMemoryRequirements(device, image, &requirements);

    const VkMemoryAllocateInfo allocInfo = {
        .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext           = nullptr,

        .allocationSize  = requirements.size,
        .memoryTypeIndex = this->GetMemoryTypeIndex(requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
    };

    VkDeviceMemory memory = nullptr;
    result = vkAllocateMemory(device, &allocInfo, nullptr, &memory);
    switch (result) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        vkDestroyImage(this->device, image, nullptr);
        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        vkDestroyImage(this->device, image, nullptr);
        return Result::OutOfDeviceMemory;
    }

    default: {
        System::Panic("vkAllocateMemory failed");
    }
    }

    result = vkBindImageMemory(device, image, memory, 0);
    switch (result) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        vkDestroyImage(this->device, image, nullptr);
        vkFreeMemory(this->device, memory, nullptr);
        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        vkDestroyImage(this->device, image, nullptr);
        vkFreeMemory(this->device, memory, nullptr);
        return Result::OutOfDeviceMemory;
    }

    default: {
        System::Panic("vkBindImageMemory failed");
    }
    }

    Wrapped<VkImageView, Result> viewResult = this->CreateImageView(image, format, VK_IMAGE_VIEW_TYPE_2D, (VkImageAspectFlagBits)viewAspectMask);
    if (!viewResult.IsValid()) {
        vkDestroyImage(this->device, image, nullptr);
        vkFreeMemory(this->device, memory, nullptr);
        return viewResult.Result();
    }

    VkImageView view = viewResult.Unwrap();

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
    result = vkCreateSampler(this->device, &samplerInfo, nullptr, &sampler);
    switch (result) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        vkDestroyImageView(this->device, view, nullptr);
        vkDestroyImage(this->device, image, nullptr);
        vkFreeMemory(this->device, memory, nullptr);

        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        vkDestroyImageView(this->device, view, nullptr);
        vkDestroyImage(this->device, image, nullptr);
        vkFreeMemory(this->device, memory, nullptr);

        return Result::OutOfDeviceMemory;
    }

    default: {
        System::Panic("vkCreateSampler failed");
    }
    }

    return new Image(this, image, memory, view, sampler, width, height, 1);
}

Image::~Image() {
    vkDestroySampler(this->device->device, this->sampler, nullptr);
    vkDestroyImageView(this->device->device, this->view, nullptr);
    vkDestroyImage(this->device->device, this->image, nullptr);
    vkFreeMemory(this->device->device, this->memory, nullptr);
}

}
