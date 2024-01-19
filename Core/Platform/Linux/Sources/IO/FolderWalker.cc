// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/IO/FolderWalker.hh>

namespace Cell::IO {

Wrapped<FolderWalker*, Result> FolderWalker::Open(const System::String& path) {
    (void)(path);

    // TODO: implement

    return Result::InvalidOperation;
}

FolderWalker::~FolderWalker() {
    (void)(this->handle);

    // TODO: implement

    CELL_UNIMPLEMENTED
}

Wrapped<FolderWalkerElementData, Result> FolderWalker::GetCurrentElementDataAndAdvance() {
    // TODO: implement

    CELL_UNIMPLEMENTED
}

Result FolderWalker::Advance() {
    // TODO: implement

    CELL_UNIMPLEMENTED
}

}
