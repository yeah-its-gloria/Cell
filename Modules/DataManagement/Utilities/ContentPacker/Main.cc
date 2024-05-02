// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/Collection/List.hh>
#include <Cell/DataManagement/Archive.hh>
#include <Cell/DataManagement/Container.hh>
#include <Cell/IO/Directory.hh>
#include <Cell/IO/File.hh>
#include <Cell/System/Entry.hh>
#include <Cell/System/Log.hh>
#include <Cell/Utilities/MinMaxClamp.hh>

using namespace Cell;
using namespace Cell::DataManagement;

void CellEntry(Reference<String> parameterString) {
    String& path = parameterString.Unwrap();
    IO::Result ioResult = IO::CheckPath(path);
    if (ioResult != IO::Result::Success) {
        System::Log("Error: Content folder not found");
        return;
    }

    System::Log("Packing Content for \"%\"...", path);
    ScopedObject<IO::Directory> directory = IO::Directory::Open(path).Unwrap();

    Collection::List<String> folders = directory->EnumerateDirectories().Unwrap();
    for (const String& folder : folders) {
        System::Log("Folder -> %", folder);
    }

    Collection::List<String> markdownFiles = directory->Enumerate("*.md").Unwrap();
    for (const String& file : markdownFiles) {
        System::Log("Markdown -> %", file);
    }

    Collection::List<String> cmakeFiles = directory->Enumerate("CMakeLists.txt").Unwrap();
    for (const String& file : cmakeFiles) {
        System::Log("CMake -> %", file);
    }
}
