// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/String.hh>

#include <string.h>

namespace Cell {
using namespace StringDetails;
using namespace System;

String::String() : data(nullptr), size(0) { }

String::String(const char* CELL_NONNULL utf8, const size_t length) : data(nullptr), size(length) {
    if (length == 0) {
        this->size = strlen(utf8);
        if (this->size == 0) {
            return; // empty
        }
    }

    this->data = AllocateMemory<char>(this->size);
    CopyMemory<char>(this->data, utf8, this->size);
}

String::String(const String& string) : data(nullptr), size(string.size) {
    if (string.size > 0) {
        this->data = AllocateMemory<char>(string.size);
        CopyMemory<char>(this->data, string.data, string.size);
    }
}

String::~String() {
    if (this->size > 0) {
        FreeMemory(this->data);
    }
}

}
