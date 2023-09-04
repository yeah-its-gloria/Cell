// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::IO {

// Different modes of operation for a pipe.
enum class PipeMode : uint8_t {
    Read = 1 << 0,
    Write = 1 << 1
};

CELL_INLINE PipeMode operator |(PipeMode a, PipeMode b) {
    return (PipeMode)((uint8_t)a | (uint8_t)b);
}

CELL_INLINE PipeMode& operator |=(PipeMode& a, PipeMode b) {
    a = a | b;
    return a;
}

CELL_INLINE PipeMode operator ^(PipeMode a, PipeMode b) {
    return (PipeMode)((uint8_t)a ^ (uint8_t)b);
}

CELL_INLINE PipeMode& operator ^=(PipeMode& a, PipeMode b) {
    a = a ^ b;
    return a;
}

CELL_INLINE PipeMode operator &(PipeMode a, PipeMode b) {
    return (PipeMode)((uint8_t)a & (uint8_t)b);
}

CELL_INLINE PipeMode& operator &=(PipeMode& a, PipeMode b) {
    a = a & b;
    return a;
}

}
