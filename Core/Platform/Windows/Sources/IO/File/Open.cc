// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/IO/File.hh>
#include <Cell/System/Platform/Windows/Includes.h>

#include <stdio.h>

#include <fcntl.h>
#include <io.h>

namespace Cell::IO {

#define HAS_MODE(in) ((FileMode::in & mode) == FileMode::in)

Wrapped<File*, Result> File::Open(const String& path, const FileMode mode) {
    if (path.IsEmpty()) {
        return Result::InvalidParameters;
    }

    DWORD creationType = OPEN_EXISTING;
    if (HAS_MODE(Create)) {
        if (HAS_MODE(Overwrite) || HAS_MODE(Open)) {
            return Result::InvalidParameters;
        }

        creationType = CREATE_NEW;
    } else if (HAS_MODE(Overwrite)) {
        if (HAS_MODE(Open)) {
            return Result::InvalidParameters;
        }

        creationType = CREATE_ALWAYS | TRUNCATE_EXISTING;
    } else if (!HAS_MODE(Open) || (!HAS_MODE(Read) && !HAS_MODE(Write))) {
        return Result::InvalidParameters;
    }

    DWORD accessType = 0;
    DWORD shareType = FILE_SHARE_READ;

    if (HAS_MODE(Read)) {
        accessType |= GENERIC_READ;
    }

    if (HAS_MODE(Write)) {
        accessType |= GENERIC_WRITE;
        shareType = 0;
    }

    ScopedBlock<wchar_t> widePath = path.ToPlatformWideString();
    if ((mode & FileMode::Create) != FileMode::Create) {
        const DWORD attributes = GetFileAttributesW(widePath);
        if (attributes == INVALID_FILE_ATTRIBUTES) {
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
                System::Panic("GetFileAttributesW failed");
            }
            }
        }

        if (attributes & FILE_ATTRIBUTE_DIRECTORY || attributes & FILE_ATTRIBUTE_DEVICE) {
            return Result::InvalidOperation;
        }
    }

    const HANDLE fileHandle = CreateFileW(&widePath, accessType, shareType, nullptr, creationType, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (fileHandle == INVALID_HANDLE_VALUE) {
        switch (GetLastError()) {
        case ERROR_ACCESS_DENIED: {
            return Result::AccessDenied;
        }

        case ERROR_PATH_NOT_FOUND:
        case ERROR_FILE_NOT_FOUND:
        case ERROR_NOT_FOUND: {
            return Result::NotFound;
        }

        case ERROR_FILE_EXISTS:
        case ERROR_ALREADY_EXISTS: {
            return Result::AlreadyExists;
        }

        default: {
            System::Panic("CreateFileW failed");
        }
        }
    }

    int osFileHandleFlags = _O_RDWR;
    char descriptorMode[5] = { 0 };
    if (HAS_MODE(Read)) {
        strncat(descriptorMode, "r", 4);

        if (!HAS_MODE(Write)) {
            osFileHandleFlags = _O_RDONLY;
        }
    }

    if (HAS_MODE(Write)) {
        if (!HAS_MODE(Read)) {
            strncat(descriptorMode, "w", 4);
            osFileHandleFlags = _O_WRONLY;
        } else {
            strncat(descriptorMode, "+", 4);
        }
    }

    if (HAS_MODE(Create)) {
        osFileHandleFlags |= _O_CREAT;
    }

    osFileHandleFlags |= _O_BINARY;
    strncat(descriptorMode, "b", 4);

    FILE* descriptor = _fdopen(_open_osfhandle((intptr_t)fileHandle, osFileHandleFlags), descriptorMode);
    CELL_ASSERT(descriptor != nullptr);

    return new File((uintptr_t)descriptor);
}

}
