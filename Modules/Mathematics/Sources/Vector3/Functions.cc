// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Mathematics/Matrix4x4.hh>

#include <math.h>

namespace Cell::Mathematics {

float Vector3::Dot(const Vector3& vector) const {
    return this->x * vector.x + this->y * vector.y + this->z * vector.z;
}

float Vector3::Dot() const {
    return this->Dot(*this);
}

Vector3 Vector3::Cross(const Vector3& vector) const {
    return { this->y * vector.z - this->z * vector.y, this->z * vector.x - this->x * vector.z, this->x * vector.y - this->y * vector.x };
}

float Vector3::Magnitude() const {
    return sqrtf(this->Dot());
}

Vector3 Vector3::Normalize() const {
    float mag = this->Magnitude();
    if (mag != 0.f) {
        return this->Multiply(1.f / mag);
    }

    return { 0.f, 0.f, 0.f };
}

Vector3 Vector3::Lerp(const Vector3& target, const float multiplier) const {
    return this->Add(target.Subtract(*this).Multiply(multiplier));
}

Vector3 Vector3::Rotate(const Matrix4x4& rotation) const {
    Quaternion xRot = Quaternion(rotation.data[0]).Normalize();
    Quaternion yRot = Quaternion(rotation.data[1]).Normalize();
    Quaternion zRot = Quaternion(rotation.data[2]).Normalize();

    return Quaternion().Add(xRot.Multiply(this->x)).Add(yRot.Multiply(this->y)).Add(zRot.Multiply(this->z)).ToVector3();
}

bool Vector3::IsZero() const {
    return this->x == 0.f && this->y == 0.f && this->z == 0.f;
}

}
