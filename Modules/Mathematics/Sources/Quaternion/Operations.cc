// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Mathematics/Quaternion.hh>

#ifdef CELL_MODULES_MATHEMATICS_HARDWARE_ACCELERATE_X86
#include <Cell/Mathematics/ExtendedIntegers.hh>
#endif

namespace Cell::Mathematics {

// Direct operations

Quaternion Quaternion::Add(const Quaternion& quat) const {
    return Quaternion(this->x + quat.x, this->y + quat.y, this->z + quat.z, this->w + quat.w);
}

Quaternion Quaternion::Add(const float scalar) const {
    return Quaternion(this->x + scalar, this->y + scalar, this->z + scalar, this->w + scalar);
}

Quaternion Quaternion::Subtract(const Quaternion& quat) const {
    return Quaternion(this->x - quat.x, this->y - quat.y, this->z - quat.z, this->w - quat.w);
}

Quaternion Quaternion::Subtract(const float scalar) const {
    return Quaternion(this->x - scalar, this->y - scalar, this->z - scalar, this->w - scalar);
}

Quaternion Quaternion::Multiply(const Quaternion& quat) const {
    return Quaternion(this->x * quat.x, this->y * quat.y, this->z * quat.z, this->w * quat.w);
}

Quaternion Quaternion::Multiply(const float scalar) const {
    return Quaternion(this->x * scalar, this->y * scalar, this->z * scalar, this->w * scalar);
}

// Regular operators

Quaternion Quaternion::operator + (const Quaternion& quat) const {
    return this->Add(quat);
}

Quaternion Quaternion::operator + (const float scalar) const {
    return this->Add(Quaternion(scalar));
}

Quaternion Quaternion::operator - (const Quaternion& quat) const {
    return this->Subtract(quat);
}

Quaternion Quaternion::operator - (const float scalar) const {
    return this->Subtract(Quaternion(scalar));
}

Quaternion Quaternion::operator * (const Quaternion& quat) const {
    return this->Multiply(quat);
}

Quaternion Quaternion::operator * (const float scalar) const {
    return this->Multiply(Quaternion(scalar));
}

// Storing operators

Quaternion& Quaternion::operator += (const Quaternion& quat) {
    *this = this->Add(quat);
    return *this;
}

Quaternion& Quaternion::operator += (const float scalar) {
    *this = this->Add(Quaternion(scalar));
    return *this;
}

Quaternion& Quaternion::operator -= (const Quaternion& quat) {
    *this = this->Subtract(quat);
    return *this;
}

Quaternion& Quaternion::operator -= (const float scalar) {
    *this = this->Subtract(Quaternion(scalar));
    return *this;
}

Quaternion& Quaternion::operator *= (const Quaternion& quat) {
    *this = this->Multiply(quat);
    return *this;
}

Quaternion& Quaternion::operator *= (const float scalar) {
    *this = this->Multiply(Quaternion(scalar));
    return *this;
}

}
