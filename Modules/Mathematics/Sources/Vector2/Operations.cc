// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Mathematics/Vector2.hh>

namespace Cell::Mathematics {

// Direct operations

Vector2 Vector2::Add(const Vector2& vector) const {
    return { this->x + vector.x, this->y + vector.y };
}

Vector2 Vector2::Add(const float scalar) const {
    return this->Add(Vector2(scalar));
}

Vector2 Vector2::Subtract(const Vector2& vector) const {
    return { this->x - vector.x, this->y - vector.y };
}

Vector2 Vector2::Subtract(const float scalar) const {
    return this->Subtract(Vector2(scalar));
}

Vector2 Vector2::Multiply(const Vector2& vector) const {
    return { this->x * vector.x, this->y * vector.y };
}

Vector2 Vector2::Multiply(const float scalar) const {
    return this->Multiply(Vector2(scalar));
}

// Regular operators

Vector2 Vector2::operator + (const Vector2& vector) const {
    return this->Add(vector);
}

Vector2 Vector2::operator + (const float scalar) const {
    return this->Add(Vector2(scalar));
}

Vector2 Vector2::operator - (const Vector2& vector) const {
    return this->Subtract(vector);
}

Vector2 Vector2::operator - (const float scalar) const {
    return this->Subtract(Vector2(scalar));
}

Vector2 Vector2::operator * (const Vector2& vector) const {
    return this->Multiply(vector);
}

Vector2 Vector2::operator * (const float scalar) const {
    return this->Multiply(Vector2(scalar));
}

// Storing operators

Vector2& Vector2::operator += (const Vector2& vector) {
    *this = this->Add(vector);
    return *this;
}

Vector2& Vector2::operator += (const float scalar) {
    *this = this->Add(Vector2(scalar));
    return *this;
}

Vector2& Vector2::operator -= (const Vector2& vector) {
    *this = this->Subtract(vector);
    return *this;
}

Vector2& Vector2::operator -= (const float scalar) {
    *this = this->Subtract(Vector2(scalar));
    return *this;
}

Vector2& Vector2::operator *= (const Vector2& vector) {
    *this = this->Multiply(vector);
    return *this;
}

Vector2& Vector2::operator *= (const float scalar) {
    *this = this->Multiply(Vector2(scalar));
    return *this;
}

}
