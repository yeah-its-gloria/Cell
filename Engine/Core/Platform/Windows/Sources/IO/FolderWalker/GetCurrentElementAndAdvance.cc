// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Instance.hh"

#include <Cell/System/Panic.hh>

namespace Cell::IO {

Wrapped<FolderWalkerElementData, Result> FolderWalker::GetCurrentElementDataAndAdvance() {
    FolderWalkerInstance* instance = (FolderWalkerInstance*)this->handle;

    FolderWalkerElementData data = {
        .fileName = System::String::FromPlatformWideString(instance->data.cFileName).Unwrap(),
        .isFolder = (instance->data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0
    };

    Result result = this->Advance();
    if (result != Result::Success) {
        return result;
    }

    return data;
}

}
