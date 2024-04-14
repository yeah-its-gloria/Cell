// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/String.hh>

#include <string.h>

namespace Cell {
using namespace Memory;
using namespace StringDetails;

Result String::Append(const String& string) {
    if (string.IsEmpty()) {
        return Result::IsEmpty;
    }

    if (this->size == 0) {
        this->data = Allocate<char>(string.size);
    } else {
        Reallocate<char>(this->data, this->size + string.size);
    }

    Copy<char>(this->data + this->size, string.data, string.size);

    this->size += string.size;
    return Result::Success;
}

Result String::Append(const char* input) {
    const size_t dataSize = StringDetails::RawStringSize(input);
    if (dataSize == 0) {
        return Result::Success;
    }

    if (this->size == 0) {
        this->data = Allocate<char>(dataSize);
    } else {
        Reallocate<char>(this->data, this->size + dataSize);
    }

    Copy<char>(this->data + this->size, input, dataSize);
    this->size += dataSize;

    return Result::Success;
}

void String::Clear() {
    if (this->size == 0) {
        return;
    }

    Free(this->data);

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
