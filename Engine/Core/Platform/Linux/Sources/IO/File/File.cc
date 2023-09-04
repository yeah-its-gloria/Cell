// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/IO/File.hh>
#include <Cell/System/Panic.hh>

#include <errno.h>
#include <stdio.h>

namespace Cell::IO {

File::~File() {
    fclose((FILE*)this->handle);
}

Result File::Read(IBlock& data, const size_t offset) {
    if (offset >= INT64_MAX) {
        return Result::InvalidParameters;
    }

    FILE* file = (FILE*)this->handle;

    const int seekResult = fseeko64(file, (int64_t)offset, SEEK_SET);
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

    const size_t read = fread(data.Pointer(), data.BlockSize(), data.Count(), file);
    if (read != data.Count()) {
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
    if (offset >= INT64_MAX) {
        return Result::InvalidParameters;
    }

    FILE* file = (FILE*)this->handle;

    const int seekResult = fseeko64(file, (int64_t)offset, SEEK_SET);
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

    const size_t writeResult = fwrite(data.Pointer(), data.BlockSize(), data.Count(), file);
    if (writeResult != data.Count()) {
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
    FILE* file = (FILE*)this->handle;

    const int seekResult = fseeko64(file, 0, SEEK_END);
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

    return size;
}

}
