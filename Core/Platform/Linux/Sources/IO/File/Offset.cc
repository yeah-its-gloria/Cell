// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/IO/File.hh>
#include <Cell/System/Panic.hh>

#include <errno.h>
#include <stdio.h>

namespace Cell::IO {

Wrapped<size_t, Result> File::GetOffset() {
    FILE* file = (FILE*)this->handle;

    const int64_t size = ftello64(file);
    if (size == -1) {
        switch (ferror(file)) {
        case EFBIG: {
            return Result::InvalidParameters;
        }

        case ENOMEM: {
            return Result::NotEnoughMemory;
        }

        default: {
            System::Panic("ftello64 failed");
        }
        }
    }

    return (size_t)size;
}

Result File::SetOffset(const size_t offset) {
    if (offset > INT64_MAX) {
        return Result::InvalidParameters;
    }

    FILE* file = (FILE*)this->handle;

    const int result = fseeko64(file, (int64_t)offset, SEEK_SET);
    if (result != 0) {
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

    return Result::Success;
}

}
