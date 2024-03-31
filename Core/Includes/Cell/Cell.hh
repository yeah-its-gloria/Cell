// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <stddef.h> // IWYU pragma: export
#include <stdint.h> // IWYU pragma: export

namespace Cell {

// Base class type for Cell.
class Object {
public:
    constexpr ~Object() = default;

    CELL_FUNCTION static void* CELL_NONNULL operator new(size_t size);
    CELL_FUNCTION static void operator delete(void* CELL_NONNULL memory, size_t size);

protected:
    constexpr Object() = default;
};

// Single instanced object, which doesn't allow copying.
class NoCopyObject : public Object {
public:
    constexpr NoCopyObject(NoCopyObject&) = delete;
    constexpr ~NoCopyObject() = default;

protected:
    constexpr NoCopyObject() = default;
};

}
