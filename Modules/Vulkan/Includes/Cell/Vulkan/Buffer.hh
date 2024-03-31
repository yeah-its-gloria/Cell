// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Memory/Block.hh>
#include <Cell/Vulkan/Device.hh>

namespace Cell::Vulkan {

// Represents an arbitrary data buffer.
class Buffer : public NoCopyObject {
friend Device;
friend class Pipeline;

public:
    // Destructs the buffer and frees its contents.
    CELL_FUNCTION ~Buffer();

    // Maps the buffer to the given address.
    CELL_FUNCTION Result Map(void*& address, const uint64_t size, const uint64_t offset = 0);

    // Unmaps the buffer.
    CELL_FUNCTION void Unmap();

    // Utility function to copy a whole block at once.
    CELL_FUNCTION Result Copy(const Memory::IBlock& data, const uint64_t offset = 0);

    // Returns the owning device.
    CELL_FUNCTION inline Device* GetDevice() { return this->device; }

    // Returns a handle to the buffer object.
    CELL_FUNCTION inline VkBuffer GetBufferHandle() { return this->buffer; }

private:
    CELL_FUNCTION_INTERNAL Buffer(Device* dev, VkBuffer buffer, VkDeviceMemory mem) : device(dev), buffer(buffer), memory(mem), isMapped(false) { }

    Device* device;
    VkBuffer buffer;
    VkDeviceMemory memory;

    bool isMapped;
};

}
