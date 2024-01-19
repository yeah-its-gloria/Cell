// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::Mathematics {

class Vector4 final : public Object {
public:
    // Rightwards value.
    float x;

    // Downwards value.
    float y;

    // Forwards value.
    float z;

    // Movement or speed value (generally).
    float w;

    // Creates a zeroed vector.
    CELL_INLINE Vector4() : x(0), y(0), z(0), w(0) { }

    // Creates a fully described vector.
    CELL_INLINE Vector4(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) { }

    // Creates a vector with all the values set to one specific value.
    CELL_INLINE Vector4(const float value) : x(value), y(value), z(value), w(value) { }

    // Scales the vector.
    CELL_FUNCTION Vector4 operator *(const float scale) const;

    // TODO: implement functions
};

}
