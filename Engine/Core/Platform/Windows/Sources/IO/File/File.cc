// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/IO/File.hh>
#include <Cell/System/Panic.hh>
#include <Cell/System/Platform/Windows/Includes.h>

#include <stdio.h>

#include <io.h>

namespace Cell::IO {

File::~File() {
    fclose((FILE*)this->handle);
}

Result File::Read(IBlock& data, const size_t offset) {
    FILE* file = (FILE*)this->handle;

    const int seekResult = _fseeki64(file, (int64_t)offset, SEEK_SET);
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
            System::Panic("_fseeki64 failed");
        }
        }
    }

    const size_t readCount = fread(data.Pointer(), 1, data.ByteSize(), file);
    if (readCount != data.ByteSize()) {
        if (feof(file) != 0) {
            return Result::ReachedEnd;
        }

        switch (ferror(file)) {
        case EFBIG: {
            return Result::InsufficientStorage;
        }

        case ENOMEM: {
            return Result::NotEnoughMemory;
        }

        default: {
            System::Panic("fread failed");
        }
        }
    }

    return Result::Success;
}

Result File::Write(const IBlock& data, const size_t offset) {
    FILE* file = (FILE*)this->handle;

    const int seekResult = _fseeki64(file, (int64_t)offset, SEEK_SET);
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
            System::Panic("_fseeki64 failed");
        }
        }
    }

    const size_t writeCount = fwrite(data.Pointer(), 1, data.ByteSize(), file);
    if (writeCount != data.ByteSize()) {
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
            System::Panic("fwrite failed");
        }
        }
    }

    return Result::Success;
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
    const int no = _fileno((FILE*)this->handle);
    if (no < 1) {
        switch (GetLastError()) {
        default: {
            System::Panic("_fileno failed");
        }
        }
    }

    HANDLE file = (HANDLE)_get_osfhandle(no);
    if (file == FALSE) {
        switch (GetLastError()) {
        default: {
            System::Panic("_get_osfhandle failed");
        }
        }
    }

    uint64_t size = 0;
    const BOOL result = GetFileSizeEx(file, (LARGE_INTEGER*)&size);
    if (result == FALSE) {
        switch (GetLastError()) {
        default: {
            System::Panic("GetFileSizeEx failed");
        }
        }
    }

    return size;
}

}
