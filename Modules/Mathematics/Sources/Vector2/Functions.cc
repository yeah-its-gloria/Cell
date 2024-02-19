// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Mathematics/Vector2.hh>

#include <math.h>

namespace Cell::Mathematics {

float Vector2::Dot(const Vector2& vector) const {
    return this->x * vector.x + this->y * vector.y;
}

float Vector2::Dot() const {
    return this->Dot(*this);
}

float Vector2::Magnitude() const {
    return sqrtf(this->Dot(*this));
}

Vector2 Vector2::Normalize() const {
    float mag = this->Magnitude();
    if (mag != 0.f) {
        return this->Multiply(1.f / mag);
    }

    return { 0.f, 0.f };
}

Vector2 Vector2::Lerp(const Vector2& target, const float multiplier) const {
    return this->Add(target.Subtract(*this).Multiply(multiplier));
}

bool Vector2::IsZero() const {
    return this->x == 0.f && this->y == 0.f;
}

}
