// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/System/BlockImpl.hh>
#include <Cell/System/Panic.hh>
#include <Cell/System/String.hh>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace Cell::System {

Wrapped<String, Result> String::FromPlatformWideString(const wchar_t* input) {
    const size_t result = wcstombs(nullptr, input, 0);
    CELL_ASSERT(result > 0);

    OwnedBlock<char> stuff(result + 1);
    const size_t result2 = wcstombs(stuff, input, result);
    CELL_ASSERT(result2 == result);

    return String(stuff);
}

wchar_t* String::ToPlatformWideString() const {
    CELL_ASSERT(this->data != nullptr && this->length > 0);

    // mbstowcs desires null termination
    ScopedBlock<char> dataStr = this->ToCharPointer();

    const size_t result = mbstowcs(nullptr, dataStr, 0);
    CELL_ASSERT(result > 0);

    wchar_t* output = System::AllocateMemory<wchar_t>(result + 1);
    const size_t result2 = mbstowcs(output, dataStr, result);
    CELL_ASSERT(result2 == result);

    return output;
}

String String::Format(const char* CELL_NONNULL format, ...) {
    CELL_ASSERT(strlen(format) < 32768);

    va_list parameters;
    va_start(parameters, format);

    char buffer[32768] = { 0 };
    vsnprintf(buffer, 32767, format, parameters);

    va_end(parameters);

    return String(buffer);
}


}
