// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Mathematics/Vector3.hh>

#include <math.h>

namespace Cell::Mathematics {

float Vector3::Dot(Vector3 vector) const {
    return this->x * vector.x + this->y * vector.y + this->z * vector.z;
}

Vector3 Vector3::Cross(Vector3 vector) const {
    return { this->y * vector.z - this->z * vector.y, this->z * vector.x - this->x * vector.z, this->x * vector.y - this->y * vector.x };
}

float Vector3::Magnitude() const {
    return sqrtf(this->Dot(*this));
}

Vector3 Vector3::Normalize() const {
    float mag = this->Magnitude();
    if (mag == 0.f) {
        return { 0.f, 0.f, 0.f };
    }

    return *this * (1.f / mag);
}

}
