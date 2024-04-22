// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Renderer/Vulkan/Buffer.hh>

namespace Cell::Renderer::Vulkan {

Wrapped<Buffer*, Result> Device::CreateBuffer(const size_t size, const VkBufferUsageFlags usage, const VkMemoryPropertyFlags type, const VkSharingMode mode) {
    const VkBufferCreateInfo bufferInfo = {
        .sType                 = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext                 = nullptr,
        .flags                 = 0,

        .size                  = size,
        .usage                 = usage,
        .sharingMode           = mode,

        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices   = nullptr
    };

    VkBuffer buffer = nullptr;
    VkResult result = vkCreateBuffer(this->device, &bufferInfo, nullptr, &buffer);
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
        System::Panic("vkCreateBuffer failed");
    }
    }

    VkMemoryRequirements requirements;
    vkGetBufferMemoryRequirements(this->device, buffer, &requirements);

    const VkMemoryAllocateInfo allocationInfo = {
        .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext           = nullptr,

        .allocationSize  = requirements.size,
        .memoryTypeIndex = this->GetMemoryTypeIndex(requirements.memoryTypeBits, type)
    };

    VkDeviceMemory memory = nullptr;
    result = vkAllocateMemory(this->device, &allocationInfo, nullptr, &memory);
    switch (result) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        vkDestroyBuffer(this->device, buffer, nullptr);
        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        vkDestroyBuffer(this->device, buffer, nullptr);
        return Result::OutOfDeviceMemory;
    }

    default: {
        System::Panic("vkAllocateMemory failed");
    }
    }

    result = vkBindBufferMemory(this->device, buffer, memory, 0);
    switch (result) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        vkDestroyBuffer(this->device, buffer, nullptr);
        vkFreeMemory(this->device, memory, nullptr);
        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        vkDestroyBuffer(this->device, buffer, nullptr);
        vkFreeMemory(this->device, memory, nullptr);
        return Result::OutOfDeviceMemory;
    }

    default: {
        System::Panic("vkBindBufferMemory failed");
    }
    }

    return new Buffer(this, buffer, memory);
}

Buffer::~Buffer() {
    vkDestroyBuffer(this->device->device, this->buffer, nullptr);
    vkFreeMemory(this->device->device, this->memory, nullptr);
}

}
