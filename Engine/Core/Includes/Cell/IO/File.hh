// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "Cell/System/Block.hh"
#include <Cell/Wrapped.hh>
#include <Cell/IO/FileMode.hh>
#include <Cell/IO/Result.hh>
#include <Cell/System/String.hh>

namespace Cell::IO {

// Represents a file within a nondescript file system.
class File : public Object {
public:
    // Opens or creates a file.
    CELL_FUNCTION static Wrapped<File*, Result> Open(const System::String& path, const FileMode mode = (FileMode)((uint8_t)FileMode::Read | (uint8_t)FileMode::Open));

    // Closes the file.
    CELL_FUNCTION ~File();

    // Reads count bytes of data in the file at the given offset (0 by default) into the block.
    CELL_FUNCTION Result Read(IBlock& data, const size_t offset = 0);

    // Writes count bytes of data from the block to the file at the given offset (0 by default).
    CELL_FUNCTION Result Write(const IBlock& data, const size_t offset = 0);

    // Flushes any buffered data, if available.
    CELL_FUNCTION Result Flush();

    // Fetches the count of the current file.
    CELL_FUNCTION Wrapped<size_t, Result> GetSize();

    // Deletes the file at the given path.
    CELL_FUNCTION static Result Delete(const System::String& path);

    // Checks if the given path is valid; e.g a file or directory is present.
    CELL_FUNCTION static Result CheckPath(const System::String& path);

    // Sets the current working directory to the given path.
    CELL_FUNCTION static Result SetWorkingDirectory(const System::String& path);

private:
    CELL_FUNCTION_INTERNAL File(const uintptr_t handle) : handle(handle) { };

    uintptr_t handle;
};

}
