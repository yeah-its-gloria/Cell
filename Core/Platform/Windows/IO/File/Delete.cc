// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/IO/File.hh>
#include <Cell/System/Platform/Windows/Includes.h>

namespace Cell::IO {

Result Delete(const String& path) {
    if (path.IsEmpty()) {
        return Result::InvalidParameters;
    }

    ScopedBlock<wchar_t> widePath = path.ToPlatformWideString();
    const BOOL result = DeleteFileW(&widePath);
    if (result == FALSE) {
        switch (GetLastError()) {
        case ERROR_ACCESS_DENIED: {
            return Result::AccessDenied;
        }

        case ERROR_PATH_NOT_FOUND:
        case ERROR_FILE_NOT_FOUND:
        case ERROR_NOT_FOUND: {
            return Result::NotFound;
        }

        default: {
            System::Panic("DeleteFileW failed");
        }
        }
    }

    return Result::Success;
}

}
