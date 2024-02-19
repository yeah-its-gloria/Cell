// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Mathematics/Quaternion.hh>

#include <math.h>

namespace Cell::Mathematics {

float Quaternion::Dot(const Quaternion& quat) const {
    return this->x * quat.x + this->y * quat.y + this->z * quat.z + this->w * quat.w;
}

float Quaternion::Dot() const {
    return this->Dot(*this);
}

float Quaternion::Magnitude() const {
    return sqrtf(this->Dot());
}

Quaternion Quaternion::Normalize() const {
    float mag = this->Magnitude();
    if (mag != 0.f) {
        return this->Multiply(1.f / mag);
    }

    return { 0.f, 0.f, 0.f, 0.f };
}

Vector3 Quaternion::ToVector3() const {
    return { this->x, this->y, this->z };
}

}
