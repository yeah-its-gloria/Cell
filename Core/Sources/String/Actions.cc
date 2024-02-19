// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/String.hh>

#include <string.h>

namespace Cell {
using namespace StringDetails;
using namespace System;

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
    const size_t dataSize = StringDetails::RawStringSize(data);

    if (this->IsEmpty()) {
        this->data = AllocateMemory<char>(dataSize);
    } else {
        ReallocateMemory<char>(&this->data, this->size + dataSize);
    }

    CopyMemory<char>(this->data + this->size, data, dataSize);
    this->size += dataSize;

    return Result::Success;
}

void String::Clear() {
    if (this->size == 0) {
        return;
    }

    FreeMemory(this->data);

    this->size = 0;
    this->data = nullptr;
}

Wrapped<String, Result> String::Substring(const size_t offset, const size_t length) const {
    if (offset + length > this->size) {
        return Result::InvalidParameters;
    }

    return String(this->data + offset, length);
}

}
