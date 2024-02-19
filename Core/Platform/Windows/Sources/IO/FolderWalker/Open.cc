// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Instance.hh"

#include <Cell/Scoped.hh>
#include <Cell/System/Panic.hh>

namespace Cell::IO {

Wrapped<FolderWalker*, Result> FolderWalker::Open(const String& path) {
    if (path.IsEmpty()) {
        return Result::InvalidParameters;
    }

    String pathModified = path;
    if (!pathModified.EndsWith("\\")) {
        pathModified += "\\";
    }

    if (!pathModified.EndsWith("**")) {
        pathModified += "**";
    }

    ScopedBlock<wchar_t> widePath = pathModified.ToPlatformWideString();

    WIN32_FIND_DATAW findData = { };
    HANDLE find = FindFirstFileExW(&widePath, FindExInfoBasic, &findData, FindExSearchNameMatch, nullptr, FIND_FIRST_EX_LARGE_FETCH);
    if (find == INVALID_HANDLE_VALUE) {
        switch (GetLastError()) {
        case ERROR_FILE_NOT_FOUND: {
            return Result::NotFound;
        }

        default: {
            System::Panic("FindFirstFileExW failed");
        }
        }
    }

    FolderWalkerInstance* instance = System::AllocateMemory<FolderWalkerInstance>();

    instance->handle = find;
    instance->data = findData;

    FolderWalker* walker = new FolderWalker((uintptr_t)instance);

    Result result = Result::Success;
    for (uint8_t i = 0; i < 2; i++) {
        result = walker->Advance();
        if (result != Result::Success) {
            break;
        }
    }

    if (result != Result::Success) {
        delete walker;
        return result;
    }

    return walker;
}

}
