// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Instance.hh"

#include <Cell/System/Memory.hh>
#include <Cell/System/Panic.hh>

namespace Cell::IO {

FolderWalker::~FolderWalker() {
    FolderWalkerInstance* instance = (FolderWalkerInstance*)this->handle;

    FindClose(instance->handle);

    System::FreeMemory(instance);
}

}
