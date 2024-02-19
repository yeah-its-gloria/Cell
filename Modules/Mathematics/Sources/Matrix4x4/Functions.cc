// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Mathematics/Matrix4x4.hh>

namespace Cell::Mathematics {

Matrix4x4 Matrix4x4::Invert() const {
    Matrix4x4 out;

    float t[6];

    t[0] = this->data[2][2] * this->data[3][3] - this->data[3][2] * this->data[2][3];
    t[1] = this->data[2][1] * this->data[3][3] - this->data[3][1] * this->data[2][3];
    t[2] = this->data[2][1] * this->data[3][2] - this->data[3][1] * this->data[2][2];
    t[3] = this->data[2][0] * this->data[3][3] - this->data[3][0] * this->data[2][3];
    t[4] = this->data[2][0] * this->data[3][2] - this->data[3][0] * this->data[2][2];
    t[5] = this->data[2][0] * this->data[3][1] - this->data[3][0] * this->data[2][1];

    out.data[0][0] =  this->data[1][1] * t[0] - this->data[1][2] * t[1] + this->data[1][3] * t[2];
    out.data[1][0] =-(this->data[1][0] * t[0] - this->data[1][2] * t[3] + this->data[1][3] * t[4]);
    out.data[2][0] =  this->data[1][0] * t[1] - this->data[1][1] * t[3] + this->data[1][3] * t[5];
    out.data[3][0] =-(this->data[1][0] * t[2] - this->data[1][1] * t[4] + this->data[1][2] * t[5]);

    out.data[0][1] =-(this->data[0][1] * t[0] - this->data[0][2] * t[1] + this->data[0][3] * t[2]);
    out.data[1][1] =  this->data[0][0] * t[0] - this->data[0][2] * t[3] + this->data[0][3] * t[4];
    out.data[2][1] =-(this->data[0][0] * t[1] - this->data[0][1] * t[3] + this->data[0][3] * t[5]);
    out.data[3][1] =  this->data[0][0] * t[2] - this->data[0][1] * t[4] + this->data[0][2] * t[5];

    t[0] = this->data[1][2] * this->data[3][3] - this->data[3][2] * this->data[1][3];
    t[1] = this->data[1][1] * this->data[3][3] - this->data[3][1] * this->data[1][3];
    t[2] = this->data[1][1] * this->data[3][2] - this->data[3][1] * this->data[1][2];
    t[3] = this->data[1][0] * this->data[3][3] - this->data[3][0] * this->data[1][3];
    t[4] = this->data[1][0] * this->data[3][2] - this->data[3][0] * this->data[1][2];
    t[5] = this->data[1][0] * this->data[3][1] - this->data[3][0] * this->data[1][1];

    out.data[0][2] =  this->data[0][1] * t[0] - this->data[0][2] * t[1] + this->data[0][3] * t[2];
    out.data[1][2] =-(this->data[0][0] * t[0] - this->data[0][2] * t[3] + this->data[0][3] * t[4]);
    out.data[2][2] =  this->data[0][0] * t[1] - this->data[0][1] * t[3] + this->data[0][3] * t[5];
    out.data[3][2] =-(this->data[0][0] * t[2] - this->data[0][1] * t[4] + this->data[0][2] * t[5]);

    t[0] = this->data[1][2] * this->data[2][3] - this->data[2][2] * this->data[1][3];
    t[1] = this->data[1][1] * this->data[2][3] - this->data[2][1] * this->data[1][3];
    t[2] = this->data[1][1] * this->data[2][2] - this->data[2][1] * this->data[1][2];
    t[3] = this->data[1][0] * this->data[2][3] - this->data[2][0] * this->data[1][3];
    t[4] = this->data[1][0] * this->data[2][2] - this->data[2][0] * this->data[1][2];
    t[5] = this->data[1][0] * this->data[2][1] - this->data[2][0] * this->data[1][1];

    out.data[0][3] =-(this->data[0][1] * t[0] - this->data[0][2] * t[1] + this->data[0][3] * t[2]);
    out.data[1][3] =  this->data[0][0] * t[0] - this->data[0][2] * t[3] + this->data[0][3] * t[4];
    out.data[2][3] =-(this->data[0][0] * t[1] - this->data[0][1] * t[3] + this->data[0][3] * t[5]);
    out.data[3][3] =  this->data[0][0] * t[2] - this->data[0][1] * t[4] + this->data[0][2] * t[5];

    const float det = 1.f / (this->data[0][0] * out.data[0][0] + this->data[0][1] * out.data[1][0] + this->data[0][2] * out.data[2][0] + this->data[0][3] * out.data[3][0]);
    return out.Multiply(det);
}

}
