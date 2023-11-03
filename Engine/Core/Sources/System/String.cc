// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/System/String.hh>

#include <errno.h>
#include <stdlib.h>
#include <string.h>

namespace Cell::System {

String::String(const char* CELL_NONNULL utf8, const size_t length) {
    if (length == 0) {
        this->length = strlen(utf8);
        if (this->length == 0) {
            return; // empty
        }
    } else {
        this->length = length;
    }

    this->data = AllocateMemory<char>(this->length);
    CopyMemory<char>(this->data, utf8, this->length);
}

String::String(const String& string) {
    this->length = string.length;
    if (this->length > 0) {
        this->data = AllocateMemory<char>(this->length);
        CopyMemory<char>(this->data, string.data, this->length);
    }
}

String::~String() {
    if (this->length > 0) {
        FreeMemory(this->data);
    }
}

Result String::Append(const String& string) {
    if (string.IsEmpty()) {
        return Result::IsEmpty;
    }

    if (this->length == 0) {
        this->data = AllocateMemory<char>(string.length);
    } else {
        ReallocateMemory<char>(&this->data, this->length + string.length);
    }

    CopyMemory<char>(this->data + this->length, string.data, string.length);

    this->length += string.length;
    return Result::Success;
}

Result String::Append(const char* data) {
    size_t dataLength = strlen(data);

    if (this->IsEmpty()) {
        this->data = AllocateMemory<char>(dataLength);
    } else {
        ReallocateMemory<char>(&this->data, this->length + dataLength);
    }

    CopyMemory<char>(this->data + this->length, data, dataLength);

    this->length += dataLength;

    return Result::Success;
}

void String::Set(const char* data) {
    FreeMemory(this->data);
    if (strncmp(data, "", 2) == 0) {
        this->length = 0;
        this->data = nullptr;
        return;
    }

    this->length = strlen(data);
    this->data = AllocateMemory<char>(this->length);

    CopyMemory<char>(this->data, data, this->length);
}

// Checks if this string begins with the given substring.
bool String::BeginsWith(const String& substring) const {
    if (substring.length >= this->length) {
        return false;
    }

    return CompareMemory(this->data, substring.data, substring.length);
}

// Checks if this string ends with the given
bool String::EndsWith(const String& substring) const {
    if (substring.length >= this->length) {
        return false;
    }

    return CompareMemory(this->data + (this->length - substring.length), substring.data, substring.length);
}

Wrapped<String, Result> String::Substring(const size_t offset, const size_t length) const {
    if (offset + length > this->length) {
        return Result::InvalidParameters;
    }

    return String(this->data + offset, length);
}

Wrapped<uint64_t, Result> String::AsNumber(const bool isHex) const {
    ScopedBlock<char> terminated = this->ToCharPointer();

    // TODO: this returns 0 if the conversion couldn't be done but isn't a number that's beyond our range
    //       find a way to properly distinguish, possibly abandoning strtoull altogether
    //       alo allow specifying an offset and end position within the string)

    const uint64_t number = strtoull(terminated, nullptr, isHex ? 16 : 10);
    if (number == UINT64_MAX && errno == ERANGE) {
        return Result::ConversionFailure;
    }

    return number;
}

char* String::ToCharPointer() const {
    char* dataStr = AllocateMemory<char>(this->length + 1);
    CopyMemory<char>(dataStr, this->data, this->length);
    return dataStr;
}

bool String::operator ==(const String& other) {
    if (this->length != other.length) {
        return false;
    }

    return CompareMemory(this->data, other.data, this->length);
}

String& String::operator =(const String& input) {
    if (this == &input) {
        return *this;
    }

    this->length = input.length;
    if (this->length > 0) {
        this->data = AllocateMemory<char>(this->length);
        CopyMemory<char>(this->data, input.data, this->length);
    } else if (this->data != nullptr) {
        FreeMemory(this->data);
        this->data = nullptr;
    }

    return *this;
}

}
