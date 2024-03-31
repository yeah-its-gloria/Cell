// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "../Tools.hh"

#include <Cell/Scoped.hh>
#include <Cell/IO/File.hh>
#include <Cell/Memory/OwnedBlock.hh>

using namespace Cell;
using namespace Cell::Vulkan;

Image* VulkanToolsLoadTexture(Device* device, const String& texturePath) {
    ScopedObject<IO::File> file = IO::File::Open(texturePath).Unwrap();

    Image* image = device->CreateImage(1024, 1024).Unwrap();

    ScopedObject buffer = device->CreateBuffer(1024 * 1024 * 4, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT).Unwrap();

    const size_t imageSize = file->GetSize();
    CELL_ASSERT(imageSize == 1024 * 1024 * 4);

    Memory::OwnedBlock<uint8_t> imageData(imageSize);
    IO::Result ioResult = file->Read(imageData);
    CELL_ASSERT(ioResult == IO::Result::Success);

    Result result = buffer->Copy(imageData);
    CELL_ASSERT(result == Result::Success);

    result = image->CopyDataFromBuffer(&buffer);
    CELL_ASSERT(result == Result::Success);

    return image;
}
