// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/String.hh>
#include <Cell/Collection/List.hh>
#include <Cell/IO/Result.hh>

namespace Cell::IO {

// Directory enumerator.
class Directory : public NoCopyObject {
public:
    // Opens the directory at the given path for enumeration.
    CELL_FUNCTION static Wrapped<Directory*, Result> Open(const String& path);

    // Cleans up all used resources.
    CELL_FUNCTION ~Directory();

    // Checks for the given files with the given name filter.
    CELL_FUNCTION Wrapped<Collection::List<String>, Result> Enumerate(const String& filter, const bool useFullPaths = false);

    // Checks for subdirectories.
    CELL_FUNCTION Wrapped<Collection::List<String>, Result> EnumerateDirectories(const bool useFullPaths = false);

private:
    CELL_FUNCTION_INTERNAL Directory(uintptr_t i) : impl(i) { }

    uintptr_t impl;
};

}
