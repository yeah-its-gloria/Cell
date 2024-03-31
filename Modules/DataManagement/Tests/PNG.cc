// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/DataManagement/Texture.hh>
#include <Cell/DataManagement/zlib.hh>
#include <Cell/IO/File.hh>
#include <Cell/Memory/OwnedBlock.hh>
#include <Cell/Memory/UnownedBlock.hh>
#include <Cell/System/Entry.hh>

using namespace Cell;
using namespace Cell::DataManagement;
using namespace Cell::Memory;

void CellEntry(Reference<String> parameterString) {
    (void)(parameterString);

    ScopedObject<IO::File> file = IO::File::Open("./Modules/DataManagement/Content/TransTrueColor.png").Unwrap();
    const size_t size = file->GetSize();

    OwnedBlock<uint8_t> data(size);
    IO::Result result = file->Read(data);
    CELL_ASSERT(result == IO::Result::Success);

    ScopedObject<Texture> texture = Texture::FromPNG(data).Unwrap();

    ScopedObject<IO::File> out = IO::File::Open("./build/stuff.raw", IO::FileMode::Overwrite | IO::FileMode::Read | IO::FileMode::Write).Unwrap();

    const size_t fullSize = 1024 * 1024 * 4;
    for (size_t i = 0; i < fullSize / 128; i++) {
        result = out->Write(UnownedBlock { ((uint8_t*)texture->GetBytes()) + i * 128, 128 });
        CELL_ASSERT(result == IO::Result::Success);
    }
}


