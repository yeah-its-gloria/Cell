// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/String.hh>
#include <Cell/IO/Result.hh>
#include <Cell/Memory/Block.hh>
#include <Cell/Utilities/Preprocessor.hh>

namespace Cell::IO {

// Different modes of operation supported for files.
enum class FileMode : uint8_t {
    // Allows reading data.
    Read = 1 << 0,

    // Allows writing data.
    Write = 1 << 1,

    // Creates a new file. This will not replace an existing file.
    Create = 1 << 2,

    // Opens an existing file. This will not create a file.
    Open = 1 << 3,

    // Either creates a new file or overwrites the contents of the existing file.
    Overwrite = 1 << 4
};

CELL_ENUM_CLASS_OPERATORS(FileMode)

// Represents a file within a nondescript, path based file system.
class File : public NoCopyObject {
public:
    // Opens or creates a file.
    CELL_FUNCTION static Wrapped<File*, Result> Open(const String& path, const FileMode mode = FileMode::Read | FileMode::Open);

    // Closes the file.
    CELL_FUNCTION ~File();

    // Reads into the given block of memory.
    CELL_FUNCTION Result Read(Memory::IBlock& data);

    // Reads into the given block of memory at the given offset.
    // The offset into the file's data will be the given offset + the size of the data.
    CELL_FUNCTION Result Read(Memory::IBlock& data, const size_t offset);

    // Writes the given block of memory.
    CELL_FUNCTION Result Write(const Memory::IBlock& data);

    // Writes the given block of memory at the given offset.
    // The offset into the file's data will be the given offset + the size of the data.
    CELL_FUNCTION Result Write(const Memory::IBlock& data, const size_t offset);

    // Flushes any buffered data, if available.
    CELL_FUNCTION Result Flush();

    // Returns the size of the file in bytes.
    CELL_FUNCTION size_t GetSize() const;

    // Retrieves the current offset into the file.
    CELL_FUNCTION size_t GetOffset() const;

    // Sets the offset at which data is read and written. By default, it's reset to zero.
    CELL_FUNCTION Result SetOffset(const size_t offset = 0);

private:
    CELL_FUNCTION_INTERNAL File(uintptr_t i) : impl(i) { }

    uintptr_t impl;
};

// Deletes the file at the given path.
CELL_FUNCTION Result Delete(const String& path);

// Checks if the given path is valid; e.g a file or directory is present.
CELL_FUNCTION Result CheckPath(const String& path);

// Sets the current working directory to the given path.
CELL_FUNCTION Result SetWorkingDirectory(const String& path);

}
