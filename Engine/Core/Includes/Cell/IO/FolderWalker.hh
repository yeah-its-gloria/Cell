// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Wrapped.hh>
#include <Cell/IO/Result.hh>
#include <Cell/System/String.hh>

namespace Cell::IO {

struct FolderWalkerElementData {
    System::String fileName;
    bool isFolder;
};

// Traversal of folders within hierarchical file systems.
class FolderWalker : public Object {
public:
    // Creates a new folder walker.
    CELL_FUNCTION static Wrapped<FolderWalker*, Result> Open(const System::String& path);

    // Destructs the folder walker.
    CELL_FUNCTION ~FolderWalker();

    // Returns the data for the current element and advances to the next available element.
    CELL_FUNCTION Wrapped<FolderWalkerElementData, Result> GetCurrentElementDataAndAdvance();

private:
    CELL_FUNCTION_INTERNAL FolderWalker(const uintptr_t handle) : handle(handle) { };
    CELL_FUNCTION_INTERNAL Result Advance();

    uintptr_t handle;
};

}
