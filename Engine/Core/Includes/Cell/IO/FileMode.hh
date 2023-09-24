// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::IO {

// Different modes of operation supported for files.
enum class FileMode : uint8_t {
    // Allows reading data.
    Read = 1 << 0,

    // Allows writing data.
    Write = 1 << 1,

    // Creates a new file. This will not replace an existing file.
    Create = 1 << 2,

    // Opens an existing file. This will not create a file.
    Open = 1 << 3,

    // Either creates a new file or overwrites the contents of the existing file.
    Overwrite = 1 << 4
};

CELL_INLINE FileMode operator |(FileMode a, FileMode b) {
    return (FileMode)((uint8_t)a | (uint8_t)b);
}

CELL_INLINE FileMode& operator |=(FileMode& a, FileMode b) {
    a = a | b;
    return a;
}

CELL_INLINE FileMode operator ^(FileMode a, FileMode b) {
    return (FileMode)((uint8_t)a ^ (uint8_t)b);
}

CELL_INLINE FileMode& operator ^=(FileMode& a, FileMode b) {
    a = a ^ b;
    return a;
}

CELL_INLINE FileMode operator &(FileMode a, FileMode b) {
    return (FileMode)((uint8_t)a & (uint8_t)b);
}

CELL_INLINE FileMode& operator &=(FileMode& a, FileMode b) {
    a = a & b;
    return a;
}

}
