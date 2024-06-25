// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Mathematics/Quaternion.hh>

namespace Cell::Mathematics {

// 4 x 4 matrix.
class alignas(4) Matrix4x4 : public Object {
public:
    // Creates an empty matrix.
    CELL_FUNCTION Matrix4x4();

    // Creates a matrix from the given raw input.
    CELL_FUNCTION Matrix4x4(float in[4][4]);

    // Copies the contents of the given matrix.
    CELL_FUNCTION Matrix4x4(const Matrix4x4& input);

    // Defaulted destructor.
    CELL_FUNCTION ~Matrix4x4() = default;

    // Creates a matrix from a 3D position vector.
    CELL_NODISCARD CELL_FUNCTION static Matrix4x4 FromVector3(const Vector3 in);

    // Creates a matrix from a quaternion.
    CELL_NODISCARD CELL_FUNCTION static Matrix4x4 FromQuaternion(const Quaternion in);

    // Creates an identity matrix.
    CELL_NODISCARD CELL_FUNCTION static Matrix4x4 FromIdentity();

    // (Utility) Creates a perspective projection matrix with the given vertical FOV, resolution and clipping plane distances.
    CELL_NODISCARD CELL_FUNCTION static Matrix4x4 AsPerspective(const float fovY, const float aspect, const float nearZ, const float farZ);

    // Adds the scalar value to this matrix.
    CELL_NODISCARD CELL_FUNCTION Matrix4x4 Add(const float scalar) const;

    // Adds the other matrix to this matrix.
    CELL_NODISCARD CELL_FUNCTION Matrix4x4 Add(const Matrix4x4& matrix) const;

    // Subtracts the scalar value from this matrix.
    CELL_NODISCARD CELL_FUNCTION Matrix4x4 Subtract(const float scalar) const;

    // Subtracts the other matrix from this matrix.
    CELL_NODISCARD CELL_FUNCTION Matrix4x4 Subtract(const Matrix4x4& matrix) const;

    // Multiplies the scalar value with this matrix.
    CELL_NODISCARD CELL_FUNCTION Matrix4x4 Multiply(const float scalar) const;

    // Multiplies the other matrix with this matrix.
    CELL_NODISCARD CELL_FUNCTION Matrix4x4 Multiply(const Matrix4x4& matrix) const;

    // Returns the inversion of this matrix.
    CELL_NODISCARD CELL_FUNCTION Matrix4x4 Invert() const;

    // (Affine Transformation) Translates this matrix to the given position.
    CELL_NODISCARD CELL_FUNCTION Matrix4x4 Translate(const Vector3& position) const;

    // (Affine Transformation) Rotates the matrix with the given angle around the given axis.
    CELL_NODISCARD CELL_FUNCTION Matrix4x4 Rotate(const float angle, const Vector3& axes) const;

    // (Affine Transformation) Rotates the matrix with the given quaternion
    CELL_NODISCARD CELL_FUNCTION Matrix4x4 Rotate(const Quaternion& quaternion) const;

    // (Affine Transformation) Scales the matrix to the given 3 dimensional size.
    CELL_NODISCARD CELL_FUNCTION Matrix4x4 Scale(const Vector3& scale) const;

    // Raw matrix data.
    float data[4][4];
};

}
