// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::Memory {

// Bare memory block interface.
// Blocks are assumed to be contiguous, single areas of memory.
class IBlock : public Object {
public:
    // Retrieves a bare void pointer to the block.
    CELL_NODISCARD virtual void* AsPointer() = 0;

    // Retrieves a byte size pointer to the block.
    CELL_NODISCARD virtual uint8_t* AsBytes() = 0;

    // Retrieves a constant bare void pointer to the block.
    CELL_NODISCARD virtual const void* AsPointer() const = 0;

    // Retrieves a constant byte size pointer to the block.
    CELL_NODISCARD virtual const uint8_t* AsBytes() const = 0;

    // Resizes the block to the given size in bytes.
    virtual void Resize(const size_t size) = 0;

    // Retrieves the number of block elements.
    CELL_NODISCARD virtual size_t GetCount() const = 0;

    // Retrieves the size of a single block element.
    CELL_NODISCARD virtual size_t GetElementSize() const = 0;

    // Retrieves the current size of the block in bytes.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE inline size_t GetSize() const {
        return this->GetElementSize() * this->GetCount();
    }

protected:
    ~IBlock() = default;
};

}
