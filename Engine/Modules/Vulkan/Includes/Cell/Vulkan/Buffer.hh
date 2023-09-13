// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/System/Block.hh>
#include <Cell/Vulkan/Instance.hh>

namespace Cell::Vulkan {

class Pipeline;

// Represents an arbitrary data buffer.
class Buffer : public Object {
friend Instance;
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

    // Utility function to copy a bare block of memory.
    CELL_FUNCTION Result Copy(const void* data, const size_t size, const uint64_t offset = 0);

    // Returns the owning Vulkan instance.
    CELL_INLINE Instance* GetOwningInstance() { return this->instance; }

    // Returns a handle to the buffer object.
    CELL_INLINE VkBuffer GetBufferHandle() { return this->buffer; }

private:
    Buffer(Instance* instance, VkBuffer buffer, VkDeviceMemory memory) : instance(instance), buffer(buffer), memory(memory), isMapped(false) { }

    Instance* instance;
    VkBuffer buffer;
    VkDeviceMemory memory;

    bool isMapped;
};

}
