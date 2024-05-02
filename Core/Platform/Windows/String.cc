// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/String.hh>
#include <Cell/System/Platform/Windows/Includes.h>

namespace Cell {
using namespace StringDetails;

Wrapped<String, Result> String::FromPlatformWideString(const wchar_t* input) {
    CELL_ASSERT(input != nullptr);

    const uint32_t inputSize = (uint32_t)wcslen(input);
    if (inputSize == 1) {
        return Result::IsEmpty;
    }

    int outputSize = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, input, (int)inputSize, nullptr, 0, nullptr, nullptr);
    if (outputSize == 0) {
        return Result::InvalidFormat;
    }

    ScopedBlock<char> data = Memory::Allocate<char>(outputSize + 1);
    outputSize = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, input, (int)inputSize, data, outputSize, nullptr, nullptr);
    CELL_ASSERT(outputSize > 0);

    return String(data, outputSize);
}

wchar_t* String::ToPlatformWideString() const {
    if (this->IsEmpty()) {
        return nullptr;
    }

    const int size = this->GetSize();

    int outputSize = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, this->data, size, nullptr, 0);
    CELL_ASSERT(outputSize > 0);

    wchar_t* output = Memory::Allocate<wchar_t>(outputSize + 1);
    outputSize = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, this->data, size, output, outputSize);
    CELL_ASSERT(outputSize > 0);

    return output;
}

NSString* String::ToPlatformNSString() const {
    CELL_UNIMPLEMENTED
}

}
