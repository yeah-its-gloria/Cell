// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/DataManagement/Model.hh>
#include <Cell/IO/File.hh>
#include <Cell/Memory/OwnedBlock.hh>
#include <Cell/System/Entry.hh>

using namespace Cell;
using namespace Cell::DataManagement;
using namespace Cell::Memory;

void CellEntry(Reference<String> parameterString) {
    (void)(parameterString);

    ScopedObject<IO::File> file = IO::File::Open("./Engine/Modules/DataManagement/Tests/Content/Box.glb").Unwrap();
    const size_t size = file->GetSize();

    OwnedBlock<uint8_t> data(size);
    IO::Result result = file->Read(data);
    CELL_ASSERT(result == IO::Result::Success);

    ScopedObject<Model> model = Model::FromGLTF(data).Unwrap();
}

