// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/String.hh>
#include <Cell/IO/Result.hh>

namespace Cell::IO {

struct FolderWalkerElementData {
    String fileName;
    bool isFolder;
};

// Traversal of folders within hierarchical file systems.
class FolderWalker : public Object {
public:
    // Creates a new folder walker.
    CELL_FUNCTION static Wrapped<FolderWalker*, Result> Open(const String& path);

    // Destructs the folder walker.
    CELL_FUNCTION ~FolderWalker();

    // Returns the data for the current element and advances to the next available element.
    CELL_FUNCTION Wrapped<FolderWalkerElementData, Result> GetCurrentElementDataAndAdvance();

    CELL_NON_COPYABLE(FolderWalker)

private:
    CELL_HANDLE_CONSTRUCTOR(FolderWalker)

    CELL_FUNCTION_INTERNAL Result Advance();

    const uintptr_t handle;
};

}
