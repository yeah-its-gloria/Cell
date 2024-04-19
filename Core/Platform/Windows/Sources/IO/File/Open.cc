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

// TODO: implement create

Wrapped<File*, Result> File::Open(const String& path, const FileMode mode) {
    if (path.IsEmpty()) {
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
    if (!HAS_MODE(Overwrite)) {
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

    const HANDLE fileHandle = CreateFileW(&widePath, accessType, shareType, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (fileHandle == INVALID_HANDLE_VALUE) {
        const DWORD result = GetLastError();
        switch (result) {
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

    const int osFileHandleFlags = [&mode] {
        switch ((CELL_BASE_TYPE(FileMode))(mode & (FileMode::Read | FileMode::Write))) {
        case (CELL_BASE_TYPE(FileMode))FileMode::Read: {
            return O_BINARY | O_RDONLY;
        }

        case (CELL_BASE_TYPE(FileMode))FileMode::Write: {
            return O_BINARY | O_WRONLY;
        }

        case (CELL_BASE_TYPE(FileMode))(FileMode::Read | FileMode::Write): {
            return O_BINARY | O_RDWR;
        }

        default: {
            CELL_UNREACHABLE;
        }
        }
    }();

#pragma clang diagnostic ignored "-Wmissing-field-initializers"
#pragma clang diagnostic push

    const char* descriptorMode =  [&mode] {
        switch ((CELL_BASE_TYPE(FileMode))(mode & (FileMode::Read | FileMode::Write))) {
        case (CELL_BASE_TYPE(FileMode))(FileMode::Read): {
            return (const char*)"rb";
        }

        case (CELL_BASE_TYPE(FileMode))(FileMode::Write): {
            return (const char*)"wb";
        }

        case (CELL_BASE_TYPE(FileMode))(FileMode::Read | FileMode::Write): {
            return (const char*)"rb+";
        }

        /*case (CELL_BASE_TYPE(FileMode))(FileMode::Read | FileMode::Write):{
            return (const char*)"wb+";
        }*/

        default: {
            CELL_UNREACHABLE;
        }
        }
    }();

#pragma clang diagnostic pop

    FILE* descriptor = _fdopen(_open_osfhandle((intptr_t)fileHandle, osFileHandleFlags), descriptorMode);
    CELL_ASSERT(descriptor != nullptr);

    return new File((uintptr_t)descriptor);
}

}
