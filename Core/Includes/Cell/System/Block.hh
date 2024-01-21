// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell {

// Managed interface for blocks of memory.
class IBlock : public Object {
public:
    // Retrieves a bare pointer to the block.
    CELL_NODISCARD virtual void* Pointer() = 0;

    // Retrieves a bare constant pointer to the block.
    CELL_NODISCARD virtual const void* Pointer() const = 0;

    // Returns the number of elements in the block.
    CELL_NODISCARD virtual size_t Count() const = 0;

    // Returns the size of an element in the block.
    CELL_NODISCARD virtual size_t BlockSize() const = 0;

    // Calculates the block's full size in bytes.
    CELL_NODISCARD CELL_INLINE size_t ByteSize() const { return this->Count() * this->BlockSize(); }
};

}
