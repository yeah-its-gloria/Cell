// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Mathematics/Matrix4x4.hh>

namespace Cell::Mathematics {

Matrix4x4::Matrix4x4(const Vector3 vec) {
    this->data[0][0] = 1.f;
    this->data[0][1] = 0.f;
    this->data[0][2] = 0.f;
    this->data[0][3] = 0.f;

    this->data[1][0] = 0.f;
    this->data[1][1] = 1.f;
    this->data[1][2] = 0.f;
    this->data[1][3] = 0.f;

    this->data[2][0] = 0.f;
    this->data[2][1] = 0.f;
    this->data[2][2] = 1.f;
    this->data[2][3] = 0.f;

    this->data[3][0] = vec.x;
    this->data[3][1] = vec.y;
    this->data[3][2] = vec.z;
    this->data[3][3] = 1.f;
}

Matrix4x4::Matrix4x4(const Vector4 quat) {
    const float x2 = quat.x + quat.x;
    const float y2 = quat.y + quat.y;
    const float z2 = quat.z + quat.z;

    const float xx2 = quat.x * x2;
    const float yy2 = quat.y * y2;
    const float zz2 = quat.z * z2;

    const float yz2 = quat.y * z2;
    const float wx2 = quat.w * x2;
    const float xy2 = quat.x * y2;
    const float wz2 = quat.w * z2;
    const float xz2 = quat.x * z2;
    const float wy2 = quat.w * y2;

    this->data[0][0] = 1.f - yy2 - zz2;
    this->data[0][1] = xy2 + wz2;
    this->data[0][2] = xz2 - wy2;
    this->data[0][3] = 0.f;

    this->data[1][0] = xy2 - wz2;
    this->data[1][0] = 1.f - xx2 - zz2;
    this->data[1][0] = yz2 + wx2;
    this->data[1][0] = 0.f;

    this->data[2][0] = xz2 + wy2;
    this->data[2][1] = yz2 - wx2;
    this->data[2][2] = 1.f - xx2 - yy2;
    this->data[2][3] = 0.f;

    this->data[3][0] = 0.f;
    this->data[3][1] = 0.f;
    this->data[3][2] = 0.f;
    this->data[3][3] = 1.f;
}

}
