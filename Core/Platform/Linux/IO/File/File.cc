// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/IO/File.hh>
#include <Cell/System/Panic.hh>

#include <errno.h>
#include <stdio.h>

namespace Cell::IO {

File::~File() {
    fclose((FILE*)this->impl);
}

Result File::Flush() {
    FILE* file = (FILE*)this->impl;

    const int result = fflush(file);
    if (result != 0) {
        switch (ferror(file)) {
        case ENOMEM: {
            return Result::NotEnoughMemory;
        }

        default: {
            System::Panic("fflush failed");
        }
        }
    }

    return Result::Success;
}

size_t File::GetSize() const {
    FILE* file = (FILE*)this->impl;

    const __off64_t initial = ftello64(file);
    if (initial < 0) {
        System::Panic("ftello64 failed");
    }

    int result = fseeko64(file, 0, SEEK_END);
    if (result != 0) {
        System::Panic("fseeko64 failed");
    }

    const __off64_t size = ftello64(file);
    if (size < 0) {
        System::Panic("ftello64 failed");
    }

    result = fseeko64(file, initial, SEEK_SET);
    if (result != 0) {
        System::Panic("fseeko64 failed");
    }

    return (size_t)size;
}

}
