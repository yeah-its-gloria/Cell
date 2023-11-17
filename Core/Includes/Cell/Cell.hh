// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <stddef.h>
#include <stdint.h>

namespace Cell {

// Base class type for Cell.
class Object {
public:
    CELL_FUNCTION static void* CELL_NONNULL operator new(size_t size);
    CELL_FUNCTION static void operator delete(void* CELL_NONNULL memory, size_t size);
};

}
