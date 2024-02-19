// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Mathematics/Vector3.hh>

namespace Cell::Mathematics {

// Quaternion, mostly used for rotations.
class Quaternion : public Object {
public:
    // Creates a zeroed quaternion.
    CELL_FUNCTION Quaternion();

    // Creates a fully described quaternion.
    CELL_FUNCTION Quaternion(const float x, const float y, const float z, const float w);

    // Creates a fully described quaternion with bare values. Serves as a utility for matrices.
    CELL_FUNCTION Quaternion(const float raw[4]);

    // Creates a quaternion with all the values set to one specific value.
    CELL_FUNCTION Quaternion(const float value);

    // Creates a quaternion with a given rotational angle and axis.
    CELL_NODISCARD CELL_FUNCTION static Quaternion FromRotation(const float angle, const Vector3& axis);

    // Adds the given quaternion to the current quaternion and returns the result.
    CELL_NODISCARD CELL_FUNCTION Quaternion Add(const Quaternion& quat) const;

    // Adds the given quaternion to the current quaternion and returns the result.
    CELL_NODISCARD CELL_FUNCTION Quaternion Add(const float scalar) const;

    // Subtracts the given quaternion from the current quaternion and returns the result.
    CELL_NODISCARD CELL_FUNCTION Quaternion Subtract(const Quaternion& quat) const;

    // Subtracts the given quaternion from the current quaternion and returns the result.
    CELL_NODISCARD CELL_FUNCTION Quaternion Subtract(const float scalar) const;

    // Multiplies the current quaternion with the given quaternion and returns the result.
    CELL_NODISCARD CELL_FUNCTION Quaternion Multiply(const Quaternion& quat) const;

    // Multiplies the current quaternion with the given scalar and returns the result.
    CELL_NODISCARD CELL_FUNCTION Quaternion Multiply(const float scalar) const;

    // Calculates the dot product over this and the given quaternion.
    CELL_NODISCARD CELL_FUNCTION float Dot(const Quaternion& quat) const;

    // Calculates the dog product over this quaternion.
    CELL_NODISCARD CELL_FUNCTION float Dot() const;

    // Calculates the magnitude (length) for this quaternion.
    CELL_NODISCARD CELL_FUNCTION float Magnitude() const;

    // Normalizes this quaternion.
    CELL_NODISCARD CELL_FUNCTION Quaternion Normalize() const;

    // Converts this quaternion to a 3D vector.
    CELL_NODISCARD CELL_FUNCTION Vector3 ToVector3() const;

    // Adds the given quaternion to the current quaternion and returns the result.
    CELL_NODISCARD CELL_FUNCTION Quaternion operator + (const Quaternion& quat) const;

    // Adds the given quaternion to the current quaternion and returns the result.
    CELL_NODISCARD CELL_FUNCTION Quaternion operator + (const float scalar) const;

    // Subtracts the given quaternion from the current quaternion and returns the result.
    CELL_NODISCARD CELL_FUNCTION Quaternion operator - (const Quaternion& quat) const;

    // Subtracts the given quaternion from the current quaternion and returns the result.
    CELL_NODISCARD CELL_FUNCTION Quaternion operator - (const float scalar) const;

    // Multiplies the current quaternion with the given quaternion and returns the result.
    CELL_NODISCARD CELL_FUNCTION Quaternion operator * (const Quaternion& quat) const;

    // Multiplies the current quaternion with the given scalar and returns the result.
    CELL_NODISCARD CELL_FUNCTION Quaternion operator * (const float scalar) const;

    // Adds the given quaternion to the current quaternion and stores the result in the current quaternion.
    CELL_FUNCTION Quaternion& operator += (const Quaternion& quat);

    // Adds the given quaternion to the current quaternion and stores the result in the current quaternion.
    CELL_FUNCTION Quaternion& operator += (const float scalar);

    // Subtracts the given quaternion from the current quaternion and stores the result in the current quaternion.
    CELL_FUNCTION Quaternion& operator -= (const Quaternion& quat);

    // Subtracts the given quaternion from the current quaternion and stores the result in the current quaternion.
    CELL_FUNCTION Quaternion& operator -= (const float scalar);

    // Multiplies the current quaternion with the given quaternion and stores the result in the current quaternion.
    CELL_FUNCTION Quaternion& operator *= (const Quaternion& quat);

    // Multiplies the current quaternion with the given scalar and stores the result in the current quaternion.
    CELL_FUNCTION Quaternion& operator *= (const float scalar);

    // (Rotation) Pitch.
    float x;

    // (Rotation) Yaw.
    float y;

    // (Rotation) Roll.
    float z;

    // (TO BE DOCUMENTED)
    float w;
};

}
