// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Mathematics/Vector3.hh>

namespace Cell::Mathematics {

// Direct operations

Vector3 Vector3::Add(const Vector3& vector) const {
    return { this->x + vector.x, this->y + vector.y, this->z + vector.z };
}

Vector3 Vector3::Add(const float scalar) const {
    return this->Add(Vector3(scalar));
}

Vector3 Vector3::Subtract(const Vector3& vector) const {
    return { this->x - vector.x, this->y - vector.y, this->z - vector.z };
}

Vector3 Vector3::Subtract(const float scalar) const {
    return this->Subtract(Vector3(scalar));
}

Vector3 Vector3::Multiply(const Vector3& vector) const {
    return { this->x * vector.x, this->y * vector.y, this->z * vector.z };
}

Vector3 Vector3::Multiply(const float scalar) const {
    return this->Multiply(Vector3(scalar));
}

// Regular operators

Vector3 Vector3::operator + (const Vector3& vector) const {
    return this->Add(vector);
}

Vector3 Vector3::operator + (const float scalar) const {
    return this->Add(Vector3(scalar));
}

Vector3 Vector3::operator - (const Vector3& vector) const {
    return this->Subtract(vector);
}

Vector3 Vector3::operator - (const float scalar) const {
    return this->Subtract(Vector3(scalar));
}

Vector3 Vector3::operator * (const Vector3& vector) const {
    return this->Multiply(vector);
}

Vector3 Vector3::operator * (const float scalar) const {
    return this->Multiply(Vector3(scalar));
}

// Storing operators

Vector3& Vector3::operator += (const Vector3& vector) {
    *this = this->Add(vector);
    return *this;
}

Vector3& Vector3::operator += (const float scalar) {
    *this = this->Add(Vector3(scalar));
    return *this;
}

Vector3& Vector3::operator -= (const Vector3& vector) {
    *this = this->Subtract(vector);
    return *this;
}

Vector3& Vector3::operator -= (const float scalar) {
    *this = this->Subtract(Vector3(scalar));
    return *this;
}

Vector3& Vector3::operator *= (const Vector3& vector) {
    *this = this->Multiply(vector);
    return *this;
}

Vector3& Vector3::operator *= (const float scalar) {
    *this = this->Multiply(Vector3(scalar));
    return *this;
}

}
