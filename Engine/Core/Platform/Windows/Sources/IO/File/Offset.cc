// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/IO/File.hh>
#include <Cell/System/Panic.hh>
#include <Cell/System/Platform/Windows/Includes.h>

#include <stdio.h>

namespace Cell::IO {

Wrapped<size_t, Result> File::GetOffset() {
    FILE* file = (FILE*)this->handle;

    int64_t size = _ftelli64(file);
    if (size == -1) {
        switch (ferror(file)) {
        case EFBIG: {
            return Result::InvalidParameters;
        }

        case ENOMEM: {
            return Result::NotEnoughMemory;
        }

        default: {
            System::Panic("_fseeki64 failed");
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

    const int result = _fseeki64(file, (int64_t)offset, SEEK_SET);
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
            System::Panic("_fseeki64 failed");
        }
        }
    }

    return Result::Success;
}

}
