// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Instance.hh"

#include <Cell/System/Panic.hh>

namespace Cell::IO {

Result FolderWalker::Advance() {
    FolderWalkerInstance* instance = (FolderWalkerInstance*)this->handle;

    const DWORD result = FindNextFileW(instance->handle, &instance->data);
    if (result == 0) {
        switch (GetLastError()) {
        case S_OK: {
            break;
        }

        case ERROR_NO_MORE_FILES: {
            return Result::NoMoreElements;
        }

        default: {
            System::Panic("FindNextFileW failed");
        }
        }
    }

    return Result::Success;
}
}
