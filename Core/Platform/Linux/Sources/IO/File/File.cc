// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/IO/File.hh>
#include <Cell/System/Panic.hh>

#include <errno.h>
#include <stdio.h>

namespace Cell::IO {

File::~File() {
    fclose((FILE*)this->handle);
}

Result File::Flush() {
    FILE* file = (FILE*)this->handle;

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

Wrapped<size_t, Result> File::GetSize() {
    FILE* file = (FILE*)this->handle;

    const __off64_t initial = ftello64(file);
    if (initial < 0) {
        System::Panic("ftello64 failed");
    }

    int seekResult = fseeko64(file, 0, SEEK_END);
    if (seekResult != 0) {
        if (feof(file) != 0) {
            return Result::ReachedEnd;
        }

        switch (ferror(file)) {
        case EFBIG: {
            return Result::InvalidParameters;
        }

        case ENOMEM: {
            return Result::NotEnoughMemory;
        }

        default: {
            System::Panic("fseeko64 failed");
        }
        }
    }

    const __off64_t size = ftello64(file);
    if (size < 0) {
        System::Panic("ftello64 failed");
    }

    seekResult = fseeko64(file, initial, SEEK_SET);
    if (seekResult != 0) {
        switch (ferror(file)) {
        case EFBIG: {
            return Result::InvalidParameters;
        }

        case ENOMEM: {
            return Result::NotEnoughMemory;
        }

        default: {
            System::Panic("fseeko64 failed");
        }
        }
    }

    return size;
}

}
