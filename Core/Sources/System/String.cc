// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/System/String.hh>

#include <errno.h>
#include <stdlib.h>
#include <string.h>

namespace Cell::System {

String::String(const char* CELL_NONNULL utf8, const size_t length) {
    if (length == 0) {
        this->size = strlen(utf8);
        if (this->size == 0) {
            return; // empty
        }
    } else {
        this->size = length;
    }

    this->data = AllocateMemory<char>(this->size);
    CopyMemory<char>(this->data, utf8, this->size);
}

String::String(const String& string) {
    this->size = string.size;
    if (this->size > 0) {
        this->data = AllocateMemory<char>(this->size);
        CopyMemory<char>(this->data, string.data, this->size);
    }
}

String::~String() {
    if (this->size > 0) {
        FreeMemory(this->data);
    }
}

Result String::Append(const String& string) {
    if (string.IsEmpty()) {
        return Result::IsEmpty;
    }

    if (this->size == 0) {
        this->data = AllocateMemory<char>(string.size);
    } else {
        ReallocateMemory<char>(&this->data, this->size + string.size);
    }

    CopyMemory<char>(this->data + this->size, string.data, string.size);

    this->size += string.size;
    return Result::Success;
}

Result String::Append(const char* data) {
    size_t dataLength = strlen(data);

    if (this->IsEmpty()) {
        this->data = AllocateMemory<char>(dataLength);
    } else {
        ReallocateMemory<char>(&this->data, this->size + dataLength);
    }

    CopyMemory<char>(this->data + this->size, data, dataLength);

    this->size += dataLength;

    return Result::Success;
}

void String::Set(const char* data) {
    FreeMemory(this->data);
    if (strncmp(data, "", 2) == 0) {
        this->size = 0;
        this->data = nullptr;
        return;
    }

    this->size = strlen(data);
    this->data = AllocateMemory<char>(this->size);

    CopyMemory<char>(this->data, data, this->size);
}

// Checks if this string begins with the given substring.
bool String::BeginsWith(const String& substring) const {
    if (substring.size >= this->size) {
        return false;
    }

    return CompareMemory(this->data, substring.data, substring.size);
}

bool String::EndsWith(const String& substring) const {
    if (substring.size >= this->size) {
        return false;
    }

    return CompareMemory(this->data + (this->size - substring.size), substring.data, substring.size);
}

Wrapped<String, Result> String::Substring(const size_t offset, const size_t length) const {
    if (offset + length > this->size) {
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

size_t String::GetCount() const {
    CELL_UNIMPLEMENTED // TODO: uh lol
}

char* String::ToCharPointer() const {
    char* dataStr = AllocateMemory<char>(this->size + 1);
    CopyMemory<char>(dataStr, this->data, this->size);
    return dataStr;
}

bool String::operator == (const String& other) {
    if (this->size != other.size) {
        return false;
    }

    return CompareMemory(this->data, other.data, this->size);
}

String& String::operator = (const String& input) {
    if (this == &input) {
        return *this;
    }

    this->size = input.size;
    if (this->size > 0) {
        this->data = AllocateMemory<char>(this->size);
        CopyMemory<char>(this->data, input.data, this->size);
    } else if (this->data != nullptr) {
        FreeMemory(this->data);
        this->data = nullptr;
    }

    return *this;
}

}
