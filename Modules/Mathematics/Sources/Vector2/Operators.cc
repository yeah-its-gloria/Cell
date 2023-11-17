// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Mathematics/Vector2.hh>

namespace Cell::Mathematics {

Vector2 Vector2::operator +(Vector2 vector) const {
    return Vector2(this->x + vector.x, this->y + vector.y);
}

Vector2 Vector2::operator -(Vector2 vector) const {
    return Vector2(this->x - vector.x, this->y - vector.y);
}

Vector2 Vector2::operator *(Vector2 scale) const {
    return Vector2(this->x * scale.x, this->y * scale.y);
}

Vector2 Vector2::operator /(Vector2 scale) const {
    return Vector2(this->x / scale.x, this->y / scale.y);
}

}
