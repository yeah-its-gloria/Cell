// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/String.hh>
#include <Cell/System/Memory.hh>

namespace Cell {

size_t String::GetSize() const {
    return this->size;
}

size_t String::GetCount() const {
    return this->size; // TODO: actually calculate the number of characters
}

bool String::IsEmpty() const {
    return this->size == 0;
}

bool String::BeginsWith(const String& substring) const {
    if (substring.size >= this->size) {
        return false;
    }

    return System::CompareMemory(this->data, substring.data, substring.size);
}

bool String::EndsWith(const String& substring) const {
    if (substring.size >= this->size) {
        return false;
    }

    return System::CompareMemory(this->data + (this->size - substring.size), substring.data, substring.size);
}

}
