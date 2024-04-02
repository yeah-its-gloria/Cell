// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/IO/File.hh>

#include <errno.h>
#include <stdio.h>

namespace Cell::IO {

Result File::Read(Memory::IBlock& data) {
    FILE* file = (FILE*)this->impl;

    const size_t readCount = fread(data.AsPointer(), data.GetElementSize(), data.GetCount(), file);
    if (readCount != data.GetCount()) {
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

Result File::Read(Memory::IBlock& data, const size_t offset) {
    Result result = this->SetOffset(offset);
    if (result != Result::Success) {
        return result;
    }

    result = this->Read(data);
    if (result != Result::Success) {
        return result;
    }

    return Result::Success;
}

Result File::Write(const Memory::IBlock& data) {
    FILE* file = (FILE*)this->impl;

    const size_t writeCount = fwrite(data.AsPointer(), data.GetElementSize(), data.GetCount(), file);
    if (writeCount != data.GetCount()) {
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

Result File::Write(const Memory::IBlock& data, const size_t offset) {
    Result result = this->SetOffset(offset);
    if (result != Result::Success) {
        return result;
    }

    result = this->Write(data);
    if (result != Result::Success) {
        return result;
    }

    return Result::Success;
}

}
