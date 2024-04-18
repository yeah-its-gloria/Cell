// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/IO/File.hh>
#include <Cell/System/Panic.hh>

#include <Foundation/Foundation.h>

namespace Cell::IO {

File::~File() {
    NSFileHandle* handle = (NSFileHandle*)this->impl;

    NSError* error = nullptr;
    [handle closeAndReturnError: &error];
    CELL_ASSERT(error == nullptr);
}

size_t File::GetSize() const {
    NSFileHandle* handle = (NSFileHandle*)this->impl;

    unsigned long long size = 0;
    NSError* error = nullptr;

    [handle seekToEndReturningOffset: &size error: &error];
    CELL_ASSERT(error == nullptr);

    [handle seekToOffset: 0 error: &error];
    CELL_ASSERT(error == nullptr);

    return (size_t)size;
}

size_t File::GetOffset() const {
    NSFileHandle* handle = (NSFileHandle*)this->impl;

    unsigned long long size = 0;
    NSError* error = nullptr;

    [handle getOffset: &size error: &error];
    CELL_ASSERT(error == nullptr);

    return (size_t)size;
}

Result File::SetOffset(const size_t offset) {
    NSFileHandle* handle = (NSFileHandle*)this->impl;

    NSError* error = nullptr;
    [handle seekToOffset: offset error: &error];
    if (error != nullptr) {
    switch (error.code) {
        default: {
            System::Panic("NSFileHandle::seekToOffset failed");
        }
        }
    }

    return Result::Success;
}

Result File::Delete(const String &path) {
    if (path.IsEmpty()) {
        return Result::InvalidParameters;
    }

    NSFileManager* manager = [NSFileManager defaultManager];
    NSString* nsPath = path.ToPlatformNSString();

    NSError* error = nullptr;
    NSDictionary<NSFileAttributeKey, id>* attributes = [manager attributesOfItemAtPath: nsPath error: &error];
    if (error != nullptr) {
        switch (error.code) {
        case NSFileNoSuchFileError: {
            return Result::NotFound;
        }

        case NSFileLockingError:{
            return Result::Locked;
        }

        case NSFileReadNoPermissionError:
        case NSFileWriteNoPermissionError: {
            return Result::AccessDenied;
        }

        default: {
            System::Panic("NSFileManager::attributesOfItemAtPath failed");
        }
        }
    }

    NSString* type = [attributes fileType];
    if (type != NSFileTypeRegular) {
        return Result::InvalidOperation;
    }

    [manager removeItemAtPath: nsPath error: &error];
    if (error != nullptr) {
        switch (error.code) {
        case NSFileReadNoPermissionError:
        case NSFileWriteNoPermissionError: {
            return Result::AccessDenied;
        }

        default: {
            System::Panic("NSFileManager::removeItemAtPath failed");
        }
        }
    }

    return Result::Success;
}

}
