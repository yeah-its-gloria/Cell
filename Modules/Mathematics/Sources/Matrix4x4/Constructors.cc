// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Mathematics/Matrix4x4.hh>

namespace Cell::Mathematics {

Matrix4x4::Matrix4x4() : data { { 0.f, 0.f, 0.f, 0.f },
                                { 0.f, 0.f, 0.f, 0.f },
                                { 0.f, 0.f, 0.f, 0.f },
                                { 0.f, 0.f, 0.f, 0.f } } { }

Matrix4x4::Matrix4x4(float in[4][4]) : data { { in[0][0], in[0][1], in[0][2], in[0][3] },
                                              { in[1][0], in[1][1], in[1][2], in[1][3] },
                                              { in[2][0], in[2][1], in[2][2], in[2][3] },
                                              { in[3][0], in[3][1], in[3][2], in[3][3] } } { }

Matrix4x4::Matrix4x4(const Matrix4x4& in) : data { { in.data[0][0], in.data[0][1], in.data[0][2], in.data[0][3] },
                                                   { in.data[1][0], in.data[1][1], in.data[1][2], in.data[1][3] },
                                                   { in.data[2][0], in.data[2][1], in.data[2][2], in.data[2][3] },
                                                   { in.data[3][0], in.data[3][1], in.data[3][2], in.data[3][3] } } { }

Matrix4x4 Matrix4x4::FromVector3(const Vector3 vec) {
    return Matrix4x4((float[4][4]){ { 1.f,   0.f,   0.f,   0.f },
                                    { 0.f,   1.f,   0.f,   0.f },
                                    { 0.f,   0.f,   1.f,   0.f },
                                    { vec.x, vec.y, vec.z, 1.f } });
}

Matrix4x4 Matrix4x4::FromQuaternion(const Quaternion quat) {
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

    return Matrix4x4((float[4][4]){ { 1.f - yy2 - zz2,       xy2 + wz2,       xz2 - wy2, 0.f },
                                    {       xy2 - wz2, 1.f - xx2 - zz2,       yz2 + wx2, 0.f },
                                    {       xz2 + wy2,       yz2 - wx2, 1.f - xx2 - yy2, 0.f },
                                    {             0.f,             0.f,             0.f, 1.f } });
}

Matrix4x4 Matrix4x4::FromIdentity() {
    return Matrix4x4((float[4][4]){ { 1.f, 0.f, 0.f, 0.f },
                                    { 0.f, 1.f, 0.f, 0.f },
                                    { 0.f, 0.f, 1.f, 0.f },
                                    { 0.f, 0.f, 0.f, 1.f } });
}

}
