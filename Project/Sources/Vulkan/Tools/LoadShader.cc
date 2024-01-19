// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "../Tools.hh"
#include <Cell/IO/File.hh>
#include <Cell/Scoped.hh>
#include <Cell/System/BlockImpl.hh>

using namespace Cell;
using namespace Cell::Vulkan;

void VulkanToolsLoadShader(Pipeline* pipeline, const System::String& path) {
    ScopedObject<IO::File> file = IO::File::Open(path).Unwrap();

    const size_t size = file->GetSize().Unwrap();
    CELL_ASSERT(size % 4 == 0);

    System::OwnedBlock<uint8_t> buffer(size);
    const IO::Result ioResult = file->Read(buffer);
    CELL_ASSERT(ioResult == IO::Result::Success);

    const Result result = pipeline->AddMultiShader(buffer);
    CELL_ASSERT(result == Result::Success);
}
