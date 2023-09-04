// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "../Tools.hh"
#include <Cell/IO/File.hh>
#include <Cell/Scoped.hh>
#include <Cell/System/BlockImpl.hh>

using namespace Cell;
using namespace Cell::Vulkan;

Image* VulkanToolsLoadTexture(Instance* instance, const System::String& texturePath) {
    ScopedObject<IO::File> file = IO::File::Open(texturePath).Unwrap();

    Image* image = instance->CreateImage(1024, 1024).Unwrap();

    ScopedObject<Buffer> buffer = instance->CreateBuffer(1024 * 1024 * 4, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT).Unwrap();

    const size_t imageSize = file->GetSize().Unwrap();
    CELL_ASSERT(imageSize == 1024 * 1024 * 4);

    System::ManagedBlock<uint8_t> imageData(imageSize);
    IO::Result ioResult = file->Read(imageData);
    CELL_ASSERT(ioResult == IO::Result::Success);

    Result result = buffer->Copy(imageData);
    CELL_ASSERT(result == Result::Success);

    result = image->CopyDataFromBuffer(&buffer);
    CELL_ASSERT(result == Result::Success);

    return image;
}
