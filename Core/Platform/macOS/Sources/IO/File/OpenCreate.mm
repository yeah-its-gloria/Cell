// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Cell/System/Panic.hh"
#include <Cell/IO/File.hh>

#include <Foundation/Foundation.h>
#include <Cocoa/Cocoa.h>

namespace Cell::IO {

const char* readFailed = "NSFileHandle::fileHandleForReadingFromURL failed";
const char* writeFailed = "NSFileHandle::fileHandleForWritingToURL failed";
const char* updateFailed = "NSFileHandle::fileHandleForUpdatingURL failed";

CELL_FUNCTION_INTERNAL Wrapped<NSFileHandle*, Result> createHandle(NSString* path, const FileMode mode) {
    NSURL* pathUrl = [[NSURL alloc] initFileURLWithPath: path];

    NSError* error = nullptr;
    NSFileHandle* handle = nullptr;
    const char* failure = nullptr;

    switch (mode & (FileMode::Read | FileMode::Write)) {
    case FileMode::Read: {
        handle = [NSFileHandle fileHandleForReadingFromURL: pathUrl error: &error];
        failure = readFailed;
        break;
    }

    case FileMode::Write: {
        handle = [NSFileHandle fileHandleForWritingToURL: pathUrl error: &error];
        failure = writeFailed;
        break;
    }

    #pragma clang diagnostic ignored "-Wswitch"
    case (FileMode::Read | FileMode::Write): {
        handle = [NSFileHandle fileHandleForUpdatingURL: pathUrl error: &error];
        failure = updateFailed;
        break;
    }

    default: {
        return Result::InvalidParameters;
    }
    }

    if (error != nullptr) {
        switch (error.code) {
        case NSFileNoSuchFileError:
        case NSFileReadNoSuchFileError: {
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
            #pragma clang diagnostic ignored "-Wformat-security"
            System::Panic(failure);
        }
        }
    }

    return handle;
}

Wrapped<File*, Result> File::Open(const String& path, const FileMode mode) {
    if (path.IsEmpty()) {
        return Result::InvalidParameters;
    }

    NSString* nsPath = path.ToPlatformNSString();
    Wrapped<NSFileHandle*, Result> result = createHandle(nsPath, mode);

    if (!result.IsValid()) {
        return result.Result();
    }

    return new File((uintptr_t)result.Unwrap());
}

Wrapped<File*, Result> File::Create(const String& path, const FileMode mode) {
    if (path.IsEmpty()) {
        return Result::InvalidParameters;
    }

    NSString* nsPath = path.ToPlatformNSString();
    NSError* error = nullptr;

    if ((mode & FileMode::Overwrite) != FileMode::Overwrite) {
        NSData* data = [[NSData alloc] init];
        [data writeToFile: nsPath options: NSDataWritingWithoutOverwriting error: &error];

        if (error != nullptr) {
            switch (error.code) {
            case NSFileWriteFileExistsError: {
                return Result::AlreadyExists;
            }

            case NSFileWriteInvalidFileNameError: {
                return Result::InvalidParameters;
            }

            case NSFileWriteNoPermissionError:
            case NSFileWriteVolumeReadOnlyError: {
                return Result::AccessDenied;
            }

            case NSFileWriteOutOfSpaceError: {
                return Result::InsufficientStorage;
            }

            default: {
                System::Panic("NSData::writeToFile failed");
            }
            }
        }
    }

    Wrapped<NSFileHandle*, Result> result = createHandle(nsPath, mode);

    if (!result.IsValid()) {
        return result.Result();
    }

    return new File((uintptr_t)result.Unwrap());
}

}
