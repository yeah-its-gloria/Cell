// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/String.hh>

#include <errno.h>
#include <stdlib.h>

namespace Cell {

char* String::ToCharPointer() const {
    char* dataStr = Memory::Allocate<char>(this->size + 1);
    Memory::Copy<char>(dataStr, this->data, this->size);

    return dataStr;
}

const char* String::ToRawPointer() const {
    return this->data;
}

Wrapped<uint64_t, StringDetails::Result> String::AsNumber(const bool isHex) const {
    ScopedBlock<char> terminated = this->ToCharPointer();

    // TODO: this returns 0 if the conversion couldn't be done but isn't a number that's beyond our range
    //       find a way to properly distinguish, possibly abandoning strtoull altogether
    //       alo allow specifying an offset and end position within the string)

    uint64_t number = strtoull(terminated, nullptr, isHex ? 16 : 10);
    if (number == UINT64_MAX && errno == ERANGE) {
        return StringDetails::Result::InvalidFormat;
    }

    return number;
}

}
