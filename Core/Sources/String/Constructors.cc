// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/String.hh>

#include <string.h>

namespace Cell {
using namespace StringDetails;
using namespace Memory;

String::String() : data(nullptr), size(0) { }

String::String(const char* CELL_NONNULL utf8, const size_t length) : data(nullptr), size(length) {
    if (length == 0) {
        this->size = strlen(utf8);
        if (this->size == 0) {
            return; // empty
        }
    }

    this->data = Allocate<char>(this->size);
    Copy<char>(this->data, utf8, this->size);
}

String::String(const String& string) : data(nullptr), size(string.size) {
    if (string.size > 0) {
        this->data = Allocate<char>(string.size);
        Copy<char>(this->data, string.data, string.size);
    }
}

String::~String() {
    if (this->size > 0) {
        Free(this->data);
    }
}

}
