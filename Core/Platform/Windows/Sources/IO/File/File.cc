// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/IO/File.hh>
#include <Cell/System/Panic.hh>
#include <Cell/System/Platform/Windows/Includes.h>

#include <io.h>
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
    const int no = _fileno((FILE*)this->impl);
    CELL_ASSERT(no > 0);

    const HANDLE file = (HANDLE)_get_osfhandle(no);
    CELL_ASSERT(file != INVALID_HANDLE_VALUE);

    uint64_t size = 0;
    const BOOL result = GetFileSizeEx(file, (LARGE_INTEGER*)&size);
    CELL_ASSERT(result == TRUE);

    return size;
}

size_t File::GetOffset() const {
    FILE* file = (FILE*)this->impl;

    const size_t size = (size_t)_ftelli64(file);
    CELL_ASSERT(size != (size_t)-1);

    return size;
}

Result File::SetOffset(const size_t offset) {
    if (offset > INT64_MAX) {
        return Result::InvalidParameters;
    }

    FILE* file = (FILE*)this->impl;

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
