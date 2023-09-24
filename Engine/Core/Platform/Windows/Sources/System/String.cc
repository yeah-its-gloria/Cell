// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/System/BlockImpl.hh>
#include <Cell/System/String.hh>
#include <Cell/System/Platform/Windows/Includes.h>

namespace Cell::System {

Wrapped<String, Result> String::FromPlatformWideString(const wchar_t* input) {
    const uint32_t inputSize = (uint32_t)wcslen(input);
    if (inputSize == 1) {
        return Result::IsEmpty;
    }

    int outputSize = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, input, (int)inputSize, nullptr, 0, nullptr, nullptr);
    if (outputSize == 0) {
        return Result::ConversionFailure;
    }

    System::OwnedBlock<char> data(outputSize + 1);
    outputSize = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, input, (int)inputSize, data, outputSize, nullptr, nullptr);
    CELL_ASSERT(outputSize > 0);

    return String(data, outputSize);
}

wchar_t* String::ToPlatformWideString() const {
    if (this->IsEmpty()) {
        return nullptr;
    }

    // Win32 APIs require null termination
    ScopedBlock<char> data = this->ToCharPointer();

    int outputSize = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, &data, (int)this->GetLength(), nullptr, 0);
    CELL_ASSERT(outputSize > 0);

    wchar_t* output = System::AllocateMemory<wchar_t>(outputSize + 1);
    outputSize = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, &data, (int)this->GetLength(), output, outputSize);
    CELL_ASSERT(outputSize > 0);

    return output;
}

}
