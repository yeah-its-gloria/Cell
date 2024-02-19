// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/IO/File.hh>
#include <Cell/System/Panic.hh>
#include <Cell/System/Platform/Windows/Includes.h>

#include <io.h>
#include <stdio.h>

namespace Cell::IO {

File::~File() {
    fclose((FILE*)this->handle);
}

Result File::Flush() {
    FILE* file = (FILE*)this->handle;

    const int result = fflush(file);
    if (result != 0) {
        switch (ferror(file)) {
        case ENOMEM: {
            return Result::NotEnoughMemory;
        }

        default: {
            System::Panic("fflush failed");
        }
        }
    }

    return Result::Success;
}

Wrapped<size_t, Result> File::GetSize() {
    const int no = _fileno((FILE*)this->handle);
    if (no < 1) {
        switch (GetLastError()) {
        default: {
            System::Panic("_fileno failed");
        }
        }
    }

    const HANDLE file = (HANDLE)_get_osfhandle(no);
    if (file == FALSE) {
        switch (GetLastError()) {
        default: {
            System::Panic("_get_osfhandle failed");
        }
        }
    }

    uint64_t size = 0;
    const BOOL result = GetFileSizeEx(file, (LARGE_INTEGER*)&size);
    if (result == FALSE) {
        switch (GetLastError()) {
        default: {
            System::Panic("GetFileSizeEx failed");
        }
        }
    }

    return size;
}

}
