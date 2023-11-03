// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/IO/FolderWalker.hh>

namespace Cell::IO {

FolderWalker::~FolderWalker() {
    CELL_UNIMPLEMENTED
}

Wrapped<FolderWalker*, Result> FolderWalker::Open(const System::String& path) {
    (void)(path);

    CELL_UNIMPLEMENTED
}

Wrapped<FolderWalkerElementData, Result>  FolderWalker::GetCurrentElementDataAndAdvance() {
    (void)(this->handle);

    CELL_UNIMPLEMENTED
}

Result FolderWalker::Advance() {
    CELL_UNIMPLEMENTED
}

}
