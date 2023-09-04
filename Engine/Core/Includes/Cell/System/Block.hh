// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "Cell/Cell.hh"

namespace Cell {

// Interface for managed blocks of memory.
class IBlock : public Object {
public:
    CELL_NODISCARD virtual void* Pointer() = 0;
    CELL_NODISCARD virtual const void* Pointer() const = 0;

    CELL_NODISCARD virtual size_t Count() const = 0;
    CELL_NODISCARD virtual size_t BlockSize() const = 0;

    CELL_NODISCARD CELL_INLINE size_t ByteSize() const { return this->Count() * this->BlockSize(); }
};

}
