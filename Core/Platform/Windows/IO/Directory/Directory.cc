// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/IO/Directory.hh>
#include <Cell/System/Platform/Windows/Includes.h>

namespace Cell::IO {

Wrapped<Directory*, Result> Directory::Open(const String& path) {
    if (path.IsEmpty()) {
        return Result::InvalidParameters;
    }

    ScopedBlock<wchar_t> widePath = path.ToPlatformWideString();
    DWORD bufferSize = GetFullPathNameW(widePath, 0, nullptr, nullptr);
    if (bufferSize == 0) {
        const DWORD error = GetLastError();
        switch (error) {
        default: {
            System::Panic("GetFullPathNameW failed");
        }
        }
    }

    wchar_t* fullPath = Memory::Allocate<wchar_t>(bufferSize + 1);
    bufferSize = GetFullPathNameW(widePath, bufferSize, fullPath, nullptr);
    if (bufferSize == 0) {
        const DWORD error = GetLastError();
        switch (error) {
        default: {
            System::Panic("GetFullPathNameW failed");
        }
        }
    }

    const DWORD attributes = GetFileAttributesW(fullPath);
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        const DWORD error = GetLastError();
        switch (error) {
        case ERROR_ACCESS_DENIED: {
            Memory::Free(fullPath);
            return Result::AccessDenied;
        }

        case ERROR_PATH_NOT_FOUND:
        case ERROR_FILE_NOT_FOUND:
        case ERROR_NOT_FOUND: {
            Memory::Free(fullPath);
            return Result::NotFound;
        }

        default: {
            System::Panic("GetFileAttributesW failed");
        }
        }
    }

    if (!(attributes & FILE_ATTRIBUTE_DIRECTORY)) {
        Memory::Free(fullPath);
        return Result::InvalidOperation;
    }

    return new Directory((uintptr_t)fullPath);
}

Directory::~Directory() {
    Memory::Free((wchar_t*)this->impl);
}

Wrapped<Collection::List<String>, Result> Directory::Enumerate(const String& filter, const bool useFullPaths) {
    String path = String::FromPlatformWideString((wchar_t*)this->impl).Unwrap();

    WIN32_FIND_DATAW data = { };

    String searchPath = path + "\\" + filter;
    ScopedBlock<wchar_t> widePath = searchPath.ToPlatformWideString();
    HANDLE finder = FindFirstFileExW(widePath, FindExInfoBasic, &data, FindExSearchNameMatch, nullptr, FIND_FIRST_EX_LARGE_FETCH);
    if (finder == INVALID_HANDLE_VALUE) {
        const DWORD error = GetLastError();
        switch (error) {
        default: {
            System::Panic("FindFirstFileExW failed");
        }
        }
    }

    Collection::List<String> files;

    while (true) {
        if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            String conversion = String::FromPlatformWideString(data.cFileName).Unwrap();
            if (useFullPaths) {
                files.Append(path + "\\" + conversion);
            } else {
                files.Append(conversion);
            }
        }

        const BOOL result = FindNextFileW(finder, &data);
        if (result == FALSE) {
            const DWORD error = GetLastError();
            if (error == ERROR_NO_MORE_FILES) {
                break;
            }

            switch (error) {
            default: {
                System::Panic("FindFirstFileExW failed");
            }
            }
        }
    }

    if (files.GetCount() == 0) {
        return Result::NoMoreElements;
    }

    return files;
}

Wrapped<Collection::List<String>, Result> Directory::EnumerateDirectories(const bool useFullPaths) {
    String path = String::FromPlatformWideString((wchar_t*)this->impl).Unwrap();

    WIN32_FIND_DATAW data = { };

    String searchPath = path + "\\*";
    ScopedBlock<wchar_t> widePath = searchPath.ToPlatformWideString();
    HANDLE finder = FindFirstFileExW(widePath, FindExInfoBasic, &data, FindExSearchNameMatch, nullptr, FIND_FIRST_EX_LARGE_FETCH);
    if (finder == INVALID_HANDLE_VALUE) {
        const DWORD error = GetLastError();
        switch (error) {
        default: {
            System::Panic("FindFirstFileExW failed");
        }
        }
    }

    // skip "." and ".."
    for (size_t i = 0; i < 2; i++) {
        const BOOL result = FindNextFileW(finder, &data);
        if (result == FALSE) {
            const DWORD error = GetLastError();
            if (error == ERROR_NO_MORE_FILES) {
                CELL_ASSERT(i > 0);

                return Result::NoMoreElements;
            }

            switch (error) {
            default: {
                System::Panic("FindFirstFileExW failed");
            }
            }
        }
    }

    Collection::List<String> folders;

    while (true) {
        if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            String conversion = String::FromPlatformWideString(data.cFileName).Unwrap();
            if (useFullPaths) {
                folders.Append(path + "\\" + conversion);
            } else {
                folders.Append(conversion);
            }
        }

        const BOOL result = FindNextFileW(finder, &data);
        if (result == FALSE) {
            const DWORD error = GetLastError();
            if (error == ERROR_NO_MORE_FILES) {
                break;
            }

            switch (error) {
            default: {
                System::Panic("FindFirstFileExW failed");
            }
            }
        }
    }

    if (folders.GetCount() == 0) {
        return Result::NoMoreElements;
    }

    return folders;
}

}
