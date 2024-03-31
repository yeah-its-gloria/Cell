// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/String.hh>
#include <Cell/Memory/Allocator.hh>

namespace Cell {
using namespace System;

String& String::operator = (const String& input) {
    if (this == &input) {
        return *this;
    }

    if (this->data != nullptr) {
        Memory::Free(this->data);
        this->data = nullptr;
    }

    this->size = input.size;
    if (input.size > 0) {
        this->data = Memory::Allocate<char>(input.size);
        Memory::Copy<char>(this->data, input.data, input.size);
    }

    return *this;
}

String& String::operator = (const char* input) {
    if (this->data != nullptr) {
        Memory::Free(this->data);
        this->data = nullptr;
    }

    this->size = StringDetails::RawStringSize(input);
    if (this->size > 0) {
        this->data = Memory::Allocate<char>(this->size);
        Memory::Copy<char>(this->data, input, this->size);
    }

    return *this;
}

bool String::operator == (const String& other) const {
    if (this == &other) {
        return true;
    }

    if (this->size != other.size) {
        return false;
    }

    return Memory::Compare(this->data, other.data, this->size);
}

bool String::operator != (const String& other) const {
    return !(*this == other);
}

String String::operator + (const String& input) const {
    String out(this->data, this->size);
    out.Append(input);
    return out;
}

String String::operator + (const char* input) const {
    String out(this->data, this->size);
    out.Append(input);
    return out;
}

void String::operator += (const String& input) {
    this->Append(input);
}

void String::operator += (const char* input) {
    this->Append(input);
}

char* String::begin() {
    return this->data;
}

char* String::end() {
    return this->data + this->size;
}

}
