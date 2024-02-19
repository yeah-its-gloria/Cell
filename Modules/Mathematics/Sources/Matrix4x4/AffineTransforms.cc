// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Mathematics/Matrix4x4.hh>

namespace Cell::Mathematics {

Matrix4x4 Matrix4x4::Translate(const Vector3& position) const {
    const Quaternion x = Quaternion(this->data[0]).Multiply(position.x);
    const Quaternion y = Quaternion(this->data[1]).Multiply(position.y);
    const Quaternion z = Quaternion(this->data[2]).Multiply(position.z);

    Matrix4x4 out = *this;

    out.data[3][0] += x.x + x.y + x.z + x.w;
    out.data[3][1] += y.x + y.y + y.z + y.w;
    out.data[3][2] += z.x + z.y + z.z + z.w;

    return out;
}

Matrix4x4 Matrix4x4::Rotate(const float angle, const Vector3& axis) const {
    return Rotate(Quaternion::FromRotation(angle, axis));
}

Matrix4x4 Matrix4x4::Rotate(const Quaternion& quaternion) const {
    return Matrix4x4::FromQuaternion(quaternion).Multiply(*this);
}

Matrix4x4 Matrix4x4::Scale(const Vector3& scale) const {
    const Quaternion x = (*(Quaternion*)this->data[0]).Multiply(scale.x);
    const Quaternion y = (*(Quaternion*)this->data[0]).Multiply(scale.y);
    const Quaternion z = (*(Quaternion*)this->data[0]).Multiply(scale.z);

    return Matrix4x4((float[4][4]){ {              x.x,              x.y,              x.z,              0.f },
                                    {              y.x,              y.y,              y.z,              0.f },
                                    {              z.x,              z.y,              z.z,              0.f },
                                    { this->data[3][0], this->data[3][1], this->data[3][2], this->data[3][3] } });
}

}
