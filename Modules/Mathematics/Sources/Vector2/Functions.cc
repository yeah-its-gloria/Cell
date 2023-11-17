// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Mathematics/Vector2.hh>

#include <math.h>

namespace Cell::Mathematics {

float Vector2::Dot(Vector2 vector) const {
    return this->x * vector.x + this->y * vector.y;
}

float Vector2::Magnitude() const {
    return sqrtf(this->Dot(*this));
}

Vector2 Vector2::Normalize() const {
    float mag = this->Magnitude();
    if (mag == 0.f) {
        return { 0.f, 0.f };
    }

    return *this * (1.f / mag);
}

}
