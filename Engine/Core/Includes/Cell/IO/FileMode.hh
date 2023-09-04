// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::IO {

// Different modes of operation supported for files.
enum class FileMode : uint8_t {
    // Allows reading the contents.
    Read = 1 << 0,

    // Allows (over)writing contents.
    Write = 1 << 1,

    // Creates a new file. An existing file with the same identity will result in an error.
    Create = 1 << 2,

    // Opens an existing file. If the file cannot be found, an error will occur.
    Open = 1 << 3,

    // Opens a new file and clears it out entirely. If the file cannot be found, it'll be created.
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
