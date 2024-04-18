// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/IO/File.hh>

#include <Foundation/Foundation.h>

namespace Cell::IO {

Result File::Read(Memory::IBlock& data) {
    NSFileHandle* handle = (NSFileHandle*)this->impl;

    NSError* error = nullptr;
    NSData* nsData = [handle readDataUpToLength: data.GetSize() error: &error];
    if (error != nullptr) {
        switch (error.code) {
        case NSFileLockingError:{
            return Result::Locked;
        }

        case NSFileReadNoPermissionError: {
            return Result::AccessDenied;
        }

        case NSFileReadTooLargeError: {
            return Result::InvalidParameters;
        }

        default: {
            System::Panic("NSFileHandle::readDataUpToLength failed");
        }
        }
    }

    [nsData getBytes: data.AsPointer() length: data.GetSize()];

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
    NSFileHandle* handle = (NSFileHandle*)this->impl;

    NSData* nsData = [[NSData alloc] initWithBytesNoCopy: (void*)data.AsPointer() length: data.GetSize() deallocator: ^(void * _Nonnull, NSUInteger) { /* ... */ }];

    NSError* error = nullptr;
    const BOOL result = [handle writeData: nsData error: &error];
    if (error != nullptr) {
        switch (error.code) {
        case NSFileLockingError:{
            return Result::Locked;
        }

        case NSFileWriteNoPermissionError: {
            return Result::AccessDenied;
        }

        case NSFileWriteOutOfSpaceError: {
            return Result::InsufficientStorage;
        }

        default: {
            System::Panic("NSFileHandle::writeData failed");
        }
        }
    }

    CELL_ASSERT(result == YES);
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

Result File::Flush() {
    NSFileHandle* handle = (NSFileHandle*)this->impl;

    NSError* error = nullptr;
    [handle synchronizeAndReturnError: &error];

    CELL_ASSERT(error == nullptr);
    return Result::Success;
}

}
