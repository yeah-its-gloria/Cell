// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/DataManagement/Foreign/PNG.hh>
#include <Cell/IO/File.hh>
#include <Cell/System/BlockImpl.hh>
#include <Cell/System/Entry.hh>
#include <Cell/System/Panic.hh>

using namespace Cell;
using namespace Cell::DataManagement::Foreign;
using namespace Cell::System;

void CellEntry(Reference<String> parameterString) {
    (void)(parameterString);

    ScopedObject<IO::File> file = IO::File::Open("./Engine/Modules/DataManagement/Content/Trans.png").Unwrap();
    const size_t size = file->GetSize().Unwrap();

    OwnedBlock<uint8_t> data(size);
    IO::Result result = file->Read(data);
    CELL_ASSERT(result == IO::Result::Success);

    PNG* png = PNG::Decode(data, size).Unwrap();
    delete png;
}
