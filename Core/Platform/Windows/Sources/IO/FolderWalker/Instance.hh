// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/IO/FolderWalker.hh>
#include <Cell/System/Platform/Windows/Includes.h>

namespace Cell::IO {

struct FolderWalkerInstance {
    WIN32_FIND_DATAW data;
    HANDLE handle;
};

}

