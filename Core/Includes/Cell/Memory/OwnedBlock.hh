// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Memory/Allocator.hh>
#include <Cell/Memory/Block.hh>

namespace Cell::Memory {

// Represents a typed block of memory that owns its memory.
template <typename T> class OwnedBlock : public IBlock {
public:
    // Creates an owned, zeroed block of memory, with the given number of T elements allocated.
    CELL_FUNCTION_INTERNAL explicit OwnedBlock(const size_t count) : data(nullptr), count(count) {
        this->data = Allocate<T>(count);
    }

    // Destructs the block and frees its memory.
    CELL_FUNCTION_INTERNAL ~OwnedBlock() {
        Free(this->data);
    }

    // Resizes the block to the given number of T elements.
    CELL_FUNCTION_INTERNAL void Resize(const size_t count) override {
        this->count = count;

        Reallocate<T>(this->data, this->count);
    }

// -> IBlock

    CELL_NODISCARD CELL_FUNCTION_INTERNAL void* AsPointer() override { return this->data; }
    CELL_NODISCARD CELL_FUNCTION_INTERNAL uint8_t* AsBytes() override { return (uint8_t*)this->data; }
    CELL_NODISCARD CELL_FUNCTION_INTERNAL const void* AsPointer() const override { return this->data; }
    CELL_NODISCARD CELL_FUNCTION_INTERNAL const uint8_t* AsBytes() const override { return (const uint8_t*)this->data; }
    CELL_NODISCARD CELL_FUNCTION_INTERNAL size_t GetCount() const override { return this->count; }
    CELL_NODISCARD CELL_FUNCTION_INTERNAL size_t GetElementSize() const override { return sizeof(T); }

private:
    T* data;
    size_t count;
};

}
