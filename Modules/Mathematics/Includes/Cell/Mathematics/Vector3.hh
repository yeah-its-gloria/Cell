// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::Mathematics {

class Vector3 final : public Object {
public:
    // Rightwards value.
    float x;

    // Downwards value.
    float y;

    // Forwards value.
    float z;

    // Creates a zeroed vector.
    CELL_INLINE Vector3() : x(0), y(0), z(0) { }

    // Creates a fully described vector.
    CELL_INLINE Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) { }

    // Creates a vector with all the values set to one specific value.
    CELL_INLINE Vector3(const float value) : x(value), y(value), z(value) { }

    // Adds the value of the given vector to this vector.
    CELL_FUNCTION Vector3 operator +(const Vector3 vector) const;

    // Subtracts the value of the given vector from this vector.
    CELL_FUNCTION Vector3 operator -(const Vector3 vector) const;

    // Scales the vector.
    CELL_FUNCTION Vector3 operator *(const Vector3 scale) const;

    // Scales the vector.
    CELL_INLINE Vector3 operator *(const float scale) const { return *this * Vector3(scale); }

    // Scales the vector.
    CELL_FUNCTION Vector3 operator /(const Vector3 scale) const;

    // Scales the vector.
    CELL_INLINE Vector3 operator /(const float scale) const { return *this / Vector3(scale); }

    // Calculates the dot product between this and the given vector.
    CELL_NODISCARD CELL_FUNCTION float Dot(const Vector3 vector) const;

    // Calculates the cross product between this and the given vector.
    CELL_NODISCARD CELL_FUNCTION Vector3 Cross(const Vector3 vector) const;

    // Calculates the magnitude (length) for this vector.
    CELL_NODISCARD CELL_FUNCTION float Magnitude() const;

    // Normalizes this vector.
    CELL_NODISCARD CELL_FUNCTION Vector3 Normalize() const;

    // Calculates the dot product for this vector.
    CELL_NODISCARD CELL_INLINE float Dot() const { return this->Dot(*this); }
};

}
