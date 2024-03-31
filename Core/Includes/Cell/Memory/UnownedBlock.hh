// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Memory/Allocator.hh>
#include <Cell/Memory/Block.hh>
#include <Cell/System/Panic.hh>

namespace Cell::Memory {

// Typed block wrapper for unowned memory.
template <typename T> class UnownedBlock : public IBlock {
public:
    // Creates an unowned block of memory with the given block address and element count.
    CELL_FUNCTION_TEMPLATE explicit UnownedBlock(const T* block, const size_t count = 1) : data(block), count(count) { }

// IBlock
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE void* AsPointer() override { return (void*)this->data; }
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE uint8_t* AsBytes() override { return (uint8_t*)this->data; }
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE const void* AsPointer() const override { return (const void*)this->data; }
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE const uint8_t* AsBytes() const override { return (const uint8_t*)this->data; }
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE size_t GetCount() const override { return this->count; }
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE size_t GetElementSize() const override { return sizeof(T); }

    CELL_FUNCTION_TEMPLATE void Resize(const size_t) override { CELL_UNIMPLEMENTED }

private:
    const T* data;
    size_t count;
};

}
