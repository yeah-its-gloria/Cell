// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Tools.hh"

#include <Cell/Scoped.hh>
#include <Cell/IO/File.hh>
#include <Cell/Memory/OwnedBlock.hh>

using namespace Cell;
using namespace Cell::D3D12;

CELL_FUNCTION_INTERNAL Memory::OwnedBlock<uint8_t> LoadFileIntoBlock(const String& path) {
    ScopedObject file = IO::File::Open(path).Unwrap();

    Memory::OwnedBlock<uint8_t> buffer(file->GetSize());
    const IO::Result ioResult = file->Read(buffer);
    CELL_ASSERT(ioResult == IO::Result::Success);

    return buffer;
}

Pipeline* D3D12ToolsLoadPipeline(const String& vertexPath, const String& pixelPath, Device* device) {
    Memory::OwnedBlock<uint8_t> vertex = LoadFileIntoBlock(vertexPath);
    Memory::OwnedBlock<uint8_t> pixel = LoadFileIntoBlock(pixelPath);

    return device->CreatePipeline(vertex, pixel).Unwrap();
}
