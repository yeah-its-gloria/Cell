// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/IO/File.hh>
#include <Cell/System/Panic.hh>

#include <errno.h>
#include <stdio.h>

namespace Cell::IO {

Result File::Read(IBlock& data) {
    FILE* file = (FILE*)this->handle;

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

Result File::Read(IBlock& data, const size_t offset, const bool keepPrevious) {
    size_t previousOffset = 0;
    if (keepPrevious) {
        Wrapped<size_t, Result> offsetResult = this->GetOffset();
        if (!offsetResult.IsValid()) {
            return offsetResult.Result();
        }

        previousOffset = offsetResult.Unwrap();
    }

    Result result = this->SetOffset(offset);
    if (result != Result::Success) {
        return result;
    }

    result = this->Read(data);
    if (result != Result::Success) {
        return result;
    }

    if (keepPrevious) {
        result = this->SetOffset(previousOffset);
        if (result != Result::Success) {
            return result;
        }
    }

    return Result::Success;
}

Result File::Write(const IBlock& data) {
    FILE* file = (FILE*)this->handle;

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

Result File::Write(const IBlock& data, const size_t offset, const bool keepPrevious) {
    size_t previousOffset = 0;
    if (keepPrevious) {
        Wrapped<size_t, Result> offsetResult = this->GetOffset();
        if (!offsetResult.IsValid()) {
            return offsetResult.Result();
        }

        previousOffset = offsetResult.Unwrap();
    }

    Result result = this->SetOffset(offset);
    if (result != Result::Success) {
        return result;
    }

    result = this->Write(data);
    if (result != Result::Success) {
        return result;
    }

    if (keepPrevious) {
        result = this->SetOffset(previousOffset);
        if (result != Result::Success) {
            return result;
        }
    }

    return Result::Success;
}

}
