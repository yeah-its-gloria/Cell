// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Mathematics/Matrix4x4.hh>
#include <Cell/System/Panic.hh>

#include <math.h>

namespace Cell::Mathematics {

Matrix4x4 Matrix4x4::LookAt(const Vector3 eye, const float pitch, const float yaw) {
    (void)(eye);
    (void)(pitch);
    (void)(yaw);

    CELL_UNIMPLEMENTED

    return *this;
}

Matrix4x4 Matrix4x4::LookAt(const Vector3 eye, const Vector3 target, const Vector3 up) {
    const Vector3 z = (target - eye).Normalize();
    const Vector3 x = z.Cross(up).Normalize();
    const Vector3 y = x.Cross(z);

    this->data[0][0] = x.x;
    this->data[0][1] = y.x;
    this->data[0][2] = -z.x;
    this->data[0][3] = 0.f;

    this->data[1][0] = x.y;
    this->data[1][1] = y.y;
    this->data[1][2] = -z.y;
    this->data[1][3] = 0.f;

    this->data[2][0] = x.z;
    this->data[2][1] = y.z;
    this->data[2][2] = -z.z;
    this->data[2][3] = 0.f;

    this->data[3][0] = -x.Dot(eye);
    this->data[3][1] = -y.Dot(eye);
    this->data[3][2] = z.Dot(eye);
    this->data[3][3] = 1.f;

    return *this;
}

Matrix4x4 Matrix4x4::Perspective(const float fovY, const float aspect, const float nearZ, const float farZ) {
    const float fovTan = 1.f / tanf(fovY * 0.5f);
    const float depth = 1.f / (nearZ - farZ);

    this->data[0][0] = fovTan / aspect;
    this->data[0][1] = 0.f;
    this->data[0][2] = 0.f;
    this->data[0][3] = 0.f;

    this->data[1][0] = 0.f;
    this->data[1][1] = fovTan;
    this->data[1][2] = 0.f;
    this->data[1][3] = 0.f;

    this->data[2][0] = 0.f;
    this->data[2][1] = 0.f;
    this->data[2][2] = farZ * depth;
    this->data[2][3] = -1.f;

    this->data[3][0] = 0.f;
    this->data[3][1] = 0.f;
    this->data[3][2] = nearZ * farZ * depth;
    this->data[3][3] = 0.f;

    return *this;
}

}
