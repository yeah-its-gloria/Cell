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

    // Overwrites an existing file.
    //
    // When opening a file, it'll be created if it doesn't exist.
    // When creating a file, the call won't fail with AlreadyExists.
    Overwrite = 1 << 2
};

CELL_ENUM_CLASS_OPERATORS(FileMode)

// Represents a file within a nondescript, path based file system.
class File : public NoCopyObject {
public:
    // Opens a file.
    CELL_FUNCTION static Wrapped<File*, Result> Open(const String& path, const FileMode mode = FileMode::Read);

    // Creates a file.
    CELL_FUNCTION static Wrapped<File*, Result> Create(const String& path, const FileMode mode = FileMode::Read | FileMode::Write);

    // Deletes the file at the given path.
    CELL_FUNCTION static Result Delete(const String& path);

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

// Checks if the given path is valid; e.g a file or directory is present.
CELL_FUNCTION Result CheckPath(const String& path);

// Sets the current working directory to the given path.
CELL_FUNCTION Result SetWorkingDirectory(const String& path);

}
