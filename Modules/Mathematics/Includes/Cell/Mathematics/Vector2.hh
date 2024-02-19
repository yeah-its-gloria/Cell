// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::Mathematics {

class Vector2 final : public Object {
public:
    // Creates a zeroed vector.
    CELL_FUNCTION Vector2();

    // Creates a fully described vector.
    CELL_FUNCTION Vector2(const float x, const float y);

    // Creates a vector with all the values set to one specific value.
    CELL_FUNCTION Vector2(const float value);

    // Adds the given vector to the current vector and returns the result.
    CELL_NODISCARD CELL_FUNCTION Vector2 Add(const Vector2& vector) const;

    // Adds the given scalar to the current vector and returns the result.
    CELL_NODISCARD CELL_FUNCTION Vector2 Add(const float scalar) const;

    // Subtracts the given vector from the current vector and returns the result.
    CELL_NODISCARD CELL_FUNCTION Vector2 Subtract(const Vector2& vector) const;

    // Subtracts the given scalar from the current vector and returns the result.
    CELL_NODISCARD CELL_FUNCTION Vector2 Subtract(const float scalar) const;

    // Multiplies the current vector with the given vector and returns the result.
    CELL_NODISCARD CELL_FUNCTION Vector2 Multiply(const Vector2& vector) const;

    // Multiplies the current vector with the given scalar and returns the result.
    CELL_NODISCARD CELL_FUNCTION Vector2 Multiply(const float scalar) const;

    // Calculates the dot product between this and the given vector.
    CELL_NODISCARD CELL_FUNCTION float Dot(const Vector2& vector) const;

    // Calculates the dot product for this vector.
    CELL_NODISCARD CELL_FUNCTION float Dot() const;

    // Calculates the magnitude (length) for this vector.
    CELL_NODISCARD CELL_FUNCTION float Magnitude() const;

    // Normalizes this vector.
    CELL_NODISCARD CELL_FUNCTION Vector2 Normalize() const;

    // Linearly interpolates the current vector towards the target vector with the given multiplier.
    CELL_NODISCARD CELL_FUNCTION Vector2 Lerp(const Vector2& target, const float multiplier = 0.5f) const;

    // Returns whether this vector is at the zero point.
    CELL_NODISCARD CELL_FUNCTION bool IsZero() const;

    // Adds the given vector to the current vector and returns the result.
    CELL_NODISCARD CELL_FUNCTION Vector2 operator + (const Vector2& vector) const;

    // Adds the given scalar across the current vector and returns the result.
    CELL_NODISCARD CELL_FUNCTION Vector2 operator + (const float scalar) const;

    // Subtracts the given vector from the current vector and returns the result.
    CELL_NODISCARD CELL_FUNCTION Vector2 operator - (const Vector2& vector) const;

    // Subtracts the given scalar across the current vector and returns the result.
    CELL_NODISCARD CELL_FUNCTION Vector2 operator - (const float scalar) const;

    // Multiplies the current vector with the given vector and returns the result.
    CELL_NODISCARD CELL_FUNCTION Vector2 operator * (const Vector2& vector) const;

    // Multiplies the given scalar across the given vector and returns the result.
    CELL_NODISCARD CELL_FUNCTION Vector2 operator * (const float scalar) const;

    // Adds the given vector to the current vector and stores the result in the current vector.
    CELL_FUNCTION Vector2& operator += (const Vector2& vector);

    // Adds the given scalar across the current vector and stores the result in the current vector.
    CELL_FUNCTION Vector2& operator += (const float scalar);

    // Subtracts the given vector from the current vector and stores the result in the current vector.
    CELL_FUNCTION Vector2& operator -= (const Vector2& vector);

    // Subtracts the given scalar across the current vector and stores the result in the current vector.
    CELL_FUNCTION Vector2& operator -= (const float scalar);

    // Multiplies the current vector with the given vector and stores the result in the current vector.
    CELL_FUNCTION Vector2& operator *= (const Vector2& vector);

    // Multiplies the given scalar across the given vector and stores the result in the current vector.
    CELL_FUNCTION Vector2& operator *= (const float scalar);

    // Rightwards value.
    float x;

    // Downwards value.
    float y;
};

}
