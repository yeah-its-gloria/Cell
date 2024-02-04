// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/System/BlockImpl.hh>
#include <Cell/System/Panic.hh>
#include <Cell/System/String.hh>

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
    CELL_ASSERT(this->data != nullptr && this->size > 0);

    // mbstowcs desires null termination
    ScopedBlock<char> dataStr = this->ToCharPointer();

    const size_t result = mbstowcs(nullptr, dataStr, 0);
    CELL_ASSERT(result > 0);

    wchar_t* output = System::AllocateMemory<wchar_t>(result + 1);
    const size_t result2 = mbstowcs(output, dataStr, result);
    CELL_ASSERT(result2 == result);

    return output;
}

}
