// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
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

// Creates a constructor for types hiding implementation details with a pointer handle.
#define CELL_HANDLE_CONSTRUCTOR(T) CELL_FUNCTION_INTERNAL CELL_INLINE constexpr T(const uintptr_t handle) : handle(handle) { }

// Deletes the copy constructor, which only permits moving the object.
#define CELL_NON_COPYABLE(T) T(T& _) = delete; T(T&& _) = default;
