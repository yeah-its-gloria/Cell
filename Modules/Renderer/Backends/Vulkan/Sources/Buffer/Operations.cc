// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Memory/Allocator.hh>
#include <Cell/Renderer/Vulkan/Buffer.hh>

namespace Cell::Renderer::Vulkan {

Result Buffer::Map(void*& address, const uint64_t size, const uint64_t offset) {
    CELL_ASSERT(!this->isMapped);

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
    CELL_ASSERT(this->isMapped);

    vkUnmapMemory(this->device->device, this->memory);

    this->isMapped = false;
}

Result Buffer::Copy(const Memory::IBlock& data, const uint64_t offset) {
    CELL_ASSERT(!this->isMapped);

    void* address = nullptr;
    Result result = this->Map(address, data.GetSize(), offset);
    if (result != Result::Success) {
        return result;
    }

    Memory::Copy(address, data.AsPointer(), data.GetSize());

    this->Unmap();
    return Result::Success;
}

}
