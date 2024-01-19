// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Mathematics/Vector3.hh>

namespace Cell::Mathematics {

Vector3 Vector3::operator +(Vector3 vector) const {
    return { this->x + vector.x, this->y + vector.y, this->z + vector.z };
}

Vector3 Vector3::operator -(Vector3 vector) const {
    return { this->x - vector.x, this->y - vector.y, this->z - vector.z };
}

Vector3 Vector3::operator *(Vector3 scale) const {
    return { this->x * scale.x, this->y * scale.y, this->z * scale.z };
}

Vector3 Vector3::operator /(Vector3 scale) const {
    return { this->x / scale.x, this->y / scale.y, this->z / scale.z };
}

}
