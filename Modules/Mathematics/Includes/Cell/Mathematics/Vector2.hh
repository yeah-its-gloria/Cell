// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::Mathematics {

class Vector2 final : public Object {
public:
    // Rightwards value.
    float x;

    // Downwards value.
    float y;

    // Creates a zeroed vector.
    CELL_INLINE Vector2() : x(0), y(0) { }

    // Creates a fully described vector.
    CELL_INLINE Vector2(const float x, const float y) : x(x), y(y) { }

    // Creates a vector with all the values set to one specific value.
    CELL_INLINE Vector2(const float value) : x(value), y(value) { }

    // Adds the value of the given vector to this vector.
    CELL_FUNCTION Vector2 operator +(const Vector2 vector) const;

    // Subtracts the value of the given vector from this vector.
    CELL_FUNCTION Vector2 operator -(const Vector2 vector) const;

    // Scales the vector.
    CELL_FUNCTION Vector2 operator *(const Vector2 scale) const;

    // Scales the vector.
    CELL_INLINE Vector2 operator *(const float scale) const { return *this * Vector2(scale); }

    // Scales the vector.
    CELL_FUNCTION Vector2 operator /(const Vector2 scale) const;

    // Scales the vector.
    CELL_INLINE Vector2 operator /(const float scale) const { return *this / Vector2(scale); }

    // Compares the vector to another
    CELL_INLINE Vector2 operator ==(const Vector2 other) const { return this->x == other.x && this->y == other.y; }

    // Calculates the dot product between this and the given vector.
    CELL_NODISCARD CELL_FUNCTION float Dot(const Vector2 vector) const;

    // Calculates the magnitude (length) for this vector.
    CELL_NODISCARD CELL_FUNCTION float Magnitude() const;

    // Normalizes this vector.
    CELL_NODISCARD CELL_FUNCTION Vector2 Normalize() const;

    // Calculates the dot product for this vector.
    CELL_NODISCARD CELL_INLINE float Dot() const { return this->Dot(*this); }
};

}
