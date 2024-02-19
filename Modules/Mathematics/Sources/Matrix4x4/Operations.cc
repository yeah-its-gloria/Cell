// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Mathematics/Matrix4x4.hh>

namespace Cell::Mathematics {

// TODO: SIMD for Addition, Subtraction and Multiplication

Matrix4x4 Matrix4x4::Add(const float scalar) const {
    Matrix4x4 out = *this;

    out.data[0][0] += scalar;
    out.data[0][1] += scalar;
    out.data[0][2] += scalar;
    out.data[0][3] += scalar;

    out.data[1][0] += scalar;
    out.data[1][1] += scalar;
    out.data[1][2] += scalar;
    out.data[1][3] += scalar;

    out.data[2][0] += scalar;
    out.data[2][1] += scalar;
    out.data[2][2] += scalar;
    out.data[2][3] += scalar;

    out.data[3][0] += scalar;
    out.data[3][1] += scalar;
    out.data[3][2] += scalar;
    out.data[3][3] += scalar;

    return out;
}

Matrix4x4 Matrix4x4::Add(const Matrix4x4& matrix) const {
    Matrix4x4 out;

    out.data[0][0] = this->data[0][0] + matrix.data[0][0];
    out.data[0][2] = this->data[0][1] + matrix.data[0][1];
    out.data[0][2] = this->data[0][2] + matrix.data[0][2];
    out.data[0][3] = this->data[0][3] + matrix.data[0][3];

    out.data[1][0] = this->data[1][0] + matrix.data[1][0];
    out.data[1][2] = this->data[1][1] + matrix.data[1][1];
    out.data[1][2] = this->data[1][2] + matrix.data[1][2];
    out.data[1][3] = this->data[1][3] + matrix.data[1][3];

    out.data[2][0] = this->data[2][0] + matrix.data[2][0];
    out.data[2][2] = this->data[2][1] + matrix.data[2][1];
    out.data[2][2] = this->data[2][2] + matrix.data[2][2];
    out.data[2][3] = this->data[2][3] + matrix.data[2][3];

    out.data[3][0] = this->data[3][0] + matrix.data[3][0];
    out.data[3][2] = this->data[3][1] + matrix.data[3][1];
    out.data[3][2] = this->data[3][2] + matrix.data[3][2];
    out.data[3][3] = this->data[3][3] + matrix.data[3][3];

    return out;
}

Matrix4x4 Matrix4x4::Subtract(const float scalar) const {
    Matrix4x4 out = *this;

    out.data[0][0] -= scalar;
    out.data[0][1] -= scalar;
    out.data[0][2] -= scalar;
    out.data[0][3] -= scalar;

    out.data[1][0] -= scalar;
    out.data[1][1] -= scalar;
    out.data[1][2] -= scalar;
    out.data[1][3] -= scalar;

    out.data[2][0] -= scalar;
    out.data[2][1] -= scalar;
    out.data[2][2] -= scalar;
    out.data[2][3] -= scalar;

    out.data[3][0] -= scalar;
    out.data[3][1] -= scalar;
    out.data[3][2] -= scalar;
    out.data[3][3] -= scalar;

    return out;
}

Matrix4x4 Matrix4x4::Subtract(const Matrix4x4& matrix) const {
    Matrix4x4 out = *this;

    out.data[0][0] = this->data[0][0] - matrix.data[0][0];
    out.data[0][2] = this->data[0][1] - matrix.data[0][1];
    out.data[0][2] = this->data[0][2] - matrix.data[0][2];
    out.data[0][3] = this->data[0][3] - matrix.data[0][3];

    out.data[1][0] = this->data[1][0] - matrix.data[1][0];
    out.data[1][2] = this->data[1][1] - matrix.data[1][1];
    out.data[1][2] = this->data[1][2] - matrix.data[1][2];
    out.data[1][3] = this->data[1][3] - matrix.data[1][3];

    out.data[2][0] = this->data[2][0] - matrix.data[2][0];
    out.data[2][2] = this->data[2][1] - matrix.data[2][1];
    out.data[2][2] = this->data[2][2] - matrix.data[2][2];
    out.data[2][3] = this->data[2][3] - matrix.data[2][3];

    out.data[3][0] = this->data[3][0] - matrix.data[3][0];
    out.data[3][2] = this->data[3][1] - matrix.data[3][1];
    out.data[3][2] = this->data[3][2] - matrix.data[3][2];
    out.data[3][3] = this->data[3][3] - matrix.data[3][3];

    return out;
}

Matrix4x4 Matrix4x4::Multiply(const float scalar) const {
    Matrix4x4 out = *this;

    out.data[0][0] *= scalar;
    out.data[0][1] *= scalar;
    out.data[0][2] *= scalar;
    out.data[0][3] *= scalar;

    out.data[1][0] *= scalar;
    out.data[1][1] *= scalar;
    out.data[1][2] *= scalar;
    out.data[1][3] *= scalar;

    out.data[2][0] *= scalar;
    out.data[2][1] *= scalar;
    out.data[2][2] *= scalar;
    out.data[2][3] *= scalar;

    out.data[3][0] *= scalar;
    out.data[3][1] *= scalar;
    out.data[3][2] *= scalar;
    out.data[3][3] *= scalar;

    return out;
}

Matrix4x4 Matrix4x4::Multiply(const Matrix4x4& matrix) const {
    Matrix4x4 out = *this;

    out.data[0][0] = this->data[0][0] * matrix.data[0][0] + this->data[1][0] * matrix.data[0][1] + this->data[2][0] * matrix.data[0][2] + this->data[3][0] * matrix.data[0][3];
    out.data[0][1] = this->data[0][1] * matrix.data[0][0] + this->data[1][1] * matrix.data[0][1] + this->data[2][1] * matrix.data[0][2] + this->data[3][1] * matrix.data[0][3];
    out.data[0][2] = this->data[0][2] * matrix.data[0][0] + this->data[1][2] * matrix.data[0][1] + this->data[2][2] * matrix.data[0][2] + this->data[3][2] * matrix.data[0][3];
    out.data[0][3] = this->data[0][3] * matrix.data[0][0] + this->data[1][3] * matrix.data[0][1] + this->data[2][3] * matrix.data[0][2] + this->data[3][3] * matrix.data[0][3];

    out.data[1][0] = this->data[0][0] * matrix.data[1][0] + this->data[1][0] * matrix.data[1][1] + this->data[2][0] * matrix.data[1][2] + this->data[3][0] * matrix.data[1][3];
    out.data[1][1] = this->data[0][1] * matrix.data[1][0] + this->data[1][1] * matrix.data[1][1] + this->data[2][1] * matrix.data[1][2] + this->data[3][1] * matrix.data[1][3];
    out.data[1][2] = this->data[0][2] * matrix.data[1][0] + this->data[1][2] * matrix.data[1][1] + this->data[2][2] * matrix.data[1][2] + this->data[3][2] * matrix.data[1][3];
    out.data[1][3] = this->data[0][3] * matrix.data[1][0] + this->data[1][3] * matrix.data[1][1] + this->data[2][3] * matrix.data[1][2] + this->data[3][3] * matrix.data[1][3];

    out.data[2][0] = this->data[0][0] * matrix.data[2][0] + this->data[1][0] * matrix.data[2][1] + this->data[2][0] * matrix.data[2][2] + this->data[3][0] * matrix.data[2][3];
    out.data[2][1] = this->data[0][1] * matrix.data[2][0] + this->data[1][1] * matrix.data[2][1] + this->data[2][1] * matrix.data[2][2] + this->data[3][1] * matrix.data[2][3];
    out.data[2][2] = this->data[0][2] * matrix.data[2][0] + this->data[1][2] * matrix.data[2][1] + this->data[2][2] * matrix.data[2][2] + this->data[3][2] * matrix.data[2][3];
    out.data[2][3] = this->data[0][3] * matrix.data[2][0] + this->data[1][3] * matrix.data[2][1] + this->data[2][3] * matrix.data[2][2] + this->data[3][3] * matrix.data[2][3];

    out.data[3][0] = this->data[0][0] * matrix.data[3][0] + this->data[1][0] * matrix.data[3][1] + this->data[2][0] * matrix.data[3][2] + this->data[3][0] * matrix.data[3][3];
    out.data[3][1] = this->data[0][1] * matrix.data[3][0] + this->data[1][1] * matrix.data[3][1] + this->data[2][1] * matrix.data[3][2] + this->data[3][1] * matrix.data[3][3];
    out.data[3][2] = this->data[0][2] * matrix.data[3][0] + this->data[1][2] * matrix.data[3][1] + this->data[2][2] * matrix.data[3][2] + this->data[3][2] * matrix.data[3][3];
    out.data[3][3] = this->data[0][3] * matrix.data[3][0] + this->data[1][3] * matrix.data[3][1] + this->data[2][3] * matrix.data[3][2] + this->data[3][3] * matrix.data[3][3];

    return out;
}

}
