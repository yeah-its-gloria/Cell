// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// ntifier: BSD-2-Clause

#include <Cell/IO/File.hh>

#include <Foundation/Foundation.h>

namespace Cell::IO {

Result CheckPath(const String& path) {
    if (path.IsEmpty()) {
        return Result::InvalidParameters;
    }

    NSString* nsPath = path.ToPlatformNSString();

    NSError* error = nullptr;
    NSDictionary<NSFileAttributeKey, id>* attributes = [[NSFileManager defaultManager] attributesOfItemAtPath: nsPath error: &error];
    if (error != nullptr) {
        switch (error.code) {
        case NSFileNoSuchFileError: {
            return Result::NotFound;
        }

        case NSFileLockingError: {
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

    (void)(attributes);

    return Result::Success;
}

Result SetWorkingDirectory(const String& path) {
    if (path.IsEmpty()) {
        return Result::InvalidParameters;
    }

    Result result = CheckPath(path);
    if (result != Result::Success) {
        return result;
    }

    NSString* nsPath = path.ToPlatformNSString();

    const BOOL success = [[NSFileManager defaultManager] changeCurrentDirectoryPath: nsPath];
    CELL_ASSERT(success == YES);

    return Result::Success;
}

}
