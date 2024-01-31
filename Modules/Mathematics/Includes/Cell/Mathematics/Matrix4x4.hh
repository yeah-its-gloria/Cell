// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Mathematics/Vector3.hh>
#include <Cell/Mathematics/Vector4.hh>

namespace Cell::Mathematics {

class alignas(16) Matrix4x4 final : public Object {
public:
    float data[4][4];

    // Creates an empty matrix.
    CELL_INLINE Matrix4x4() : data { { 0.0, 0.0, 0.0, 0.0 },
                                     { 0.0, 0.0, 0.0, 0.0 },
                                     { 0.0, 0.0, 0.0, 0.0 },
                                     { 0.0, 0.0, 0.0, 0.0 } } { }

    // Creates a matrix from a 3 axis vector.
    CELL_FUNCTION Matrix4x4(const Vector3 vec);

    // Creates a matrix from a quaternion.
    CELL_FUNCTION Matrix4x4(const Vector4 quat);

    // Resets the matrix to its identity.
    CELL_FUNCTION Matrix4x4 SetToIdentity();

    // Inverts the matrix.
    CELL_FUNCTION Matrix4x4 Invert();

    // (Affine Transformation) Translates this matrix to the given position.
    CELL_FUNCTION Matrix4x4 Translate(const Vector3 position);

    // (Affine Transformation) Rotates the matrix with the given angle around the given axis.
    CELL_FUNCTION Matrix4x4 Rotate(const float angle, const Vector3 axis);

    // (Affine Transformation) Scales the matrix to the given count.
    CELL_FUNCTION Matrix4x4 Scale(const Vector3 scale);

    // (Camera/view utility) Calculates a camera view for this matrix.
    CELL_FUNCTION Matrix4x4 LookAt(const Vector3 eye, const Vector3 target, const Vector3 up);

    // (Camera/view utility) Calculates a camera view for this matrix.
    CELL_FUNCTION Matrix4x4 LookAt(const Vector3 eye, const float pitch, const float yaw);

    // (Perspective utility) Calculates perspective projection for this matrix.
    CELL_FUNCTION Matrix4x4 Perspective(const float fovY, const float aspect, const float nearZ, const float farZ);

    // Adds the scalar value to this matrix.
    CELL_FUNCTION Matrix4x4 Add(const float scalar);

    // Adds the other matrix to this matrix.
    CELL_FUNCTION Matrix4x4 Add(const Matrix4x4 matrix);

    // Subtracts the scalar value from this matrix.
    CELL_FUNCTION Matrix4x4 Subtract(const float scalar);

    // Subtracts the other matrix from this matrix.
    CELL_FUNCTION Matrix4x4 Subtract(const Matrix4x4 matrix);

    // Multiplies the scalar value with this matrix.
    CELL_FUNCTION Matrix4x4 Multiply(const float scalar);

    // Multiplies the other matrix with this matrix.
    CELL_FUNCTION Matrix4x4 Multiply(const Matrix4x4 matrix);

    // Adds the scalar value to this matrix.
    CELL_INLINE Matrix4x4 operator + (const float scalar) {
        return this->Add(scalar);
    }

    // Adds the other matrix to this matrix.
    CELL_INLINE Matrix4x4 operator + (const Matrix4x4 matrix) {
        return this->Add(matrix);
    }

    // Subtracts the scalar value from this matrix.
    CELL_INLINE Matrix4x4 operator - (const float scalar) {
        return this->Subtract(scalar);
    }

    // Subtracts the other matrix from this matrix.
    CELL_INLINE Matrix4x4 operator - (const Matrix4x4 matrix) {
        return this->Subtract(matrix);
    }

    // Multiplies the scalar value with this matrix.
    CELL_INLINE Matrix4x4 operator * (const float scalar) {
        return this->Multiply(scalar);
    }

    // Multiplies the other matrix with this matrix.
    CELL_INLINE Matrix4x4 operator * (const Matrix4x4 matrix) {
        return this->Multiply(matrix);
    }
};

}
