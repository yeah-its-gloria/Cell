// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/Buffer.hh>

namespace Cell::Vulkan {

Wrapped<Buffer*, Result> Instance::CreateBuffer(const size_t size, const VkBufferUsageFlags usage, const VkMemoryPropertyFlags type, const VkSharingMode mode) {
    if (this->device == nullptr) {
        return Result::InvalidState;
    }

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

    VkBuffer vkBuffer = nullptr;
    VkResult result = vkCreateBuffer(this->device, &bufferInfo, nullptr, &vkBuffer);
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
    vkGetBufferMemoryRequirements(this->device, vkBuffer, &requirements);

    const VkMemoryAllocateInfo allocationInfo = {
        .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext           = nullptr,

        .allocationSize  = requirements.size,
        .memoryTypeIndex = this->GetMemoryTypeIndex(vkBuffer, type)
    };

    VkDeviceMemory memory = nullptr;
    result = vkAllocateMemory(this->device, &allocationInfo, nullptr, &memory);
    switch (result) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        vkDestroyBuffer(this->device, vkBuffer, nullptr);
        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        vkDestroyBuffer(this->device, vkBuffer, nullptr);
        return Result::OutOfDeviceMemory;
    }

    default: {
        System::Panic("vkAllocateMemory failed");
    }
    }

    result = vkBindBufferMemory(this->device, vkBuffer, memory, 0);
    switch (result) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        vkDestroyBuffer(this->device, vkBuffer, nullptr);
        vkFreeMemory(this->device, memory, nullptr);
        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        vkDestroyBuffer(this->device, vkBuffer, nullptr);
        vkFreeMemory(this->device, memory, nullptr);
        return Result::OutOfDeviceMemory;
    }

    default: {
        System::Panic("vkBindBufferMemory failed");
    }
    }

    return new Buffer(this, vkBuffer, memory);
}

}
