// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/IO/File.hh>
#include <Cell/IO/FolderWalker.hh>
#include <Cell/System/Entry.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Memory.hh>
#include <Cell/System/Panic.hh>

using namespace Cell;

void CellEntry(Reference<System::IPlatform> platform, Reference<System::String> parameterString) {
    IO::Result ioResult = IO::File::CheckPath(parameterString.Unwrap());
    if (ioResult != IO::Result::Success) {
        System::Log("Error: Content folder not found");
        return;
    }

    System::Log("Packing Content...");

    size_t count = 0;

    ScopedObject<IO::FolderWalker> walker = IO::FolderWalker::Open(parameterString.Unwrap()).Unwrap();

    IO::FolderWalkerElementData data;
    for (uint8_t i = 0; i < 3; i++) {
        if (!platform.Unwrap().IsStillActive()) {
            System::Log("Aborting...");
            return;
        }

        walker->GetCurrentElementDataAndAdvance(data);
        System::Log("- %s\t%s", data.fileName.ToCharPointer(), data.isFolder ? "folder" : "file");

        if (!data.isFolder) {
            count++;
        }
    }

    System::Log("Number of files found: %llu.", count);
}
