// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Instance.hh"

#include <Cell/System/Panic.hh>

namespace Cell::IO {

Result FolderWalker::GetCurrentElementDataAndAdvance(FolderWalkerElementData& output) {
    FolderWalkerInstance* instance = (FolderWalkerInstance*)this->handle;

    output.fileName = System::String::FromPlatformWideString(instance->data.cFileName).Unwrap();
    output.isFolder = (instance->data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

    return this->Advance();
}
}
