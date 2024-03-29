// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Memory.hh>
#include <Cell/Vulkan/Buffer.hh>

namespace Cell::Vulkan {

Result Buffer::Map(void*& address, const uint64_t size, const uint64_t offset) {
    if (this->isMapped) {
        return Result::InvalidState;
    }

    if (size == 0) {
        return Result::InvalidParameters;
    }

    VkResult result = vkMapMemory(this->device->device, this->memory, offset, size, 0, &address);
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
        System::Panic("vkMapMemory failed");
    }
    }

    this->isMapped = true;
    return Result::Success;
}

void Buffer::Unmap() {
    if (!this->isMapped) {
        return;
    }

    vkUnmapMemory(this->device->device, this->memory);

    this->isMapped = false;
}

Result Buffer::Copy(const IBlock& data, const uint64_t offset) {
    if (this->isMapped) {
        return Result::InvalidState;
    }

    void* address = nullptr;
    Result result = this->Map(address, data.ByteSize(), offset);
    if (result != Result::Success) {
        return result;
    }

    System::CopyMemory(address, data.Pointer(), data.ByteSize());

    this->Unmap();
    return Result::Success;
}

}
