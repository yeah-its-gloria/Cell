// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/System/Block.hh>
#include <Cell/Vulkan/Device.hh>

namespace Cell::Vulkan {

class Pipeline;

// Represents an arbitrary data buffer.
class Buffer : public Object {
friend Device;
friend Pipeline;

public:
    // Destructs the buffer and frees its contents.
    CELL_FUNCTION ~Buffer();

    // Maps the buffer to the given address.
    CELL_FUNCTION Result Map(void*& address, const uint64_t size, const uint64_t offset = 0);

    // Unmaps the buffer.
    CELL_FUNCTION void Unmap();

    // Utility function to copy a whole block at once.
    CELL_FUNCTION Result Copy(const IBlock& data, const uint64_t offset = 0);

    // Returns the owning device.
    CELL_INLINE Device* GetDevice() { return this->device; }

    // Returns a handle to the buffer object.
    CELL_INLINE VkBuffer GetBufferHandle() { return this->buffer; }

private:
    Buffer(Device* dev, VkBuffer buffer, VkDeviceMemory mem) : device(dev), buffer(buffer), memory(mem), isMapped(false) { }

    Device* device;
    VkBuffer buffer;
    VkDeviceMemory memory;

    bool isMapped;
};

}
