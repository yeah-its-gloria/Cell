// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/Collection/List.hh>
#include <Cell/DataManagement/Archive.hh>
#include <Cell/DataManagement/Container.hh>
#include <Cell/IO/File.hh>
#include <Cell/IO/FolderWalker.hh>
#include <Cell/System/BlockImpl.hh>
#include <Cell/System/Entry.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Memory.hh>
#include <Cell/System/Panic.hh>
#include <Cell/Utilities/MinMaxClamp.hh>

using namespace Cell;
using namespace Cell::DataManagement;

void CellEntry(Reference<System::String> parameterString) {
    IO::Result ioResult = IO::CheckPath(parameterString.Unwrap());
    if (ioResult != IO::Result::Success) {
        System::Log("Error: Content folder not found");
        return;
    }

    System::Log("Packing Content...");

    ScopedObject walker = IO::FolderWalker::Open(parameterString.Unwrap()).Unwrap();

    /*Collection::List<System::String> elements;
    while (true) {
        Wrapped<IO::FolderWalkerElementData, IO::Result> result = walker->GetCurrentElementDataAndAdvance();
        if (result.Result() == IO::Result::NoMoreElements) {
            break;
        }

        IO::FolderWalkerElementData stuff = result.Unwrap();
        if (!stuff.isFolder) {
            elements.Append(stuff.fileName);
        }
    }*/

    System::String fileName = parameterString.Unwrap() + "\\data.bin"; //+ elements[0];
    ScopedObject importedFile = IO::File::Open(fileName).Unwrap();

    ScopedObject archive = IO::File::Open("archive.cel", IO::FileMode::Write | IO::FileMode::Create).Unwrap();

    ContainerHeader containerHeader = {
        .version = ContainerVersion,
        .kind = ContainerContentKind::Archive
    };

    System::CopyMemory(containerHeader.magic, ContainerMagic, 4);

    ArchiveHeader archiveHeader = {
        .elements = 1
    };

    ArchiveEntry archiveEntry = {
        .name = { 0 },
        .size = (uint32_t)importedFile->GetSize().Unwrap()
    };

    //System::CopyMemory(archiveEntry.name, elements[0].ToRawPointer(), Utilities::Minimum<size_t>(elements[0].GetLength(), 20));
    System::CopyMemory(archiveEntry.name, "data.bin", 8);

    System::OwnedBlock<uint8_t> importedFileData(archiveEntry.size);
    ioResult = importedFile->Read(importedFileData);
    CELL_ASSERT(ioResult == IO::Result::Success);

    ioResult = archive->Write(System::UnownedBlock { &containerHeader });
    CELL_ASSERT(ioResult == IO::Result::Success);

    ioResult = archive->Write(System::UnownedBlock { &archiveHeader });
    CELL_ASSERT(ioResult == IO::Result::Success);

    ioResult = archive->Write(System::UnownedBlock { &archiveEntry });
    CELL_ASSERT(ioResult == IO::Result::Success);

    ioResult = archive->Write(importedFileData);
    CELL_ASSERT(ioResult == IO::Result::Success);
}
