// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/String.hh>

namespace Cell {

Wrapped<String, StringDetails::Result> String::FromPlatformWideString(const wchar_t* input) {
    (void)(input);

    CELL_UNIMPLEMENTED
}

wchar_t* String::ToPlatformWideString() const {
    CELL_UNIMPLEMENTED
}

NSString* String::ToPlatformNSString() const {
    return [[NSString alloc] initWithBytesNoCopy: (char*)this->data length: this->size encoding: NSUTF8StringEncoding freeWhenDone: NO];
}

}
