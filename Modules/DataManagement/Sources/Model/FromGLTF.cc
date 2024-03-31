// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/DataManagement/JSON.hh>
#include <Cell/DataManagement/Model.hh>
#include <Cell/Utilities/Preprocessor.hh>
#include <Cell/Utilities/Reader.hh>

namespace Cell::DataManagement {

// NOLINTBEGIN (readability-identifier-naming)

struct CELL_PACKED(1) glTFHeader {
    uint32_t magic;
    uint32_t version;
    uint32_t size;
};

enum class glTFChunkType : uint32_t {
    JSON = 0x4e4f534a,
    Binary = 0x004e4942
};

struct CELL_PACKED(1) glTFChunkHeader {
    uint32_t chunkSize;
    glTFChunkType chunkType;
};

const uint32_t glTFHeaderMagic = 0x46546c67;// "glTF", ASCII

// NOLINTEND

Wrapped<Model*, Result> Model::FromGLTF(const Memory::IBlock& block) {
    if (block.GetSize() == 0) {
        return Result::InvalidParameters;
    }

    Utilities::Reader reader(block);

    glTFHeader header = reader.Read<glTFHeader>();
    if (header.magic != glTFHeaderMagic || header.version != 2 || header.size != block.GetSize()) {
        return Result::InvalidData;
    }

    glTFChunkHeader jsonChunkHeader = reader.Read<glTFChunkHeader>();
    if (jsonChunkHeader.chunkSize == 0 || jsonChunkHeader.chunkSize >= block.GetSize() - sizeof(glTFHeader) || jsonChunkHeader.chunkType != glTFChunkType::JSON) {
        return Result::InvalidData;
    }

    String jsonData = [&] {
        ScopedBlock<uint8_t> block = Memory::Allocate<uint8_t>(jsonChunkHeader.chunkSize);
        reader.ReadBytes(block, jsonChunkHeader.chunkSize);

        return String((char*)(uint8_t*)block, jsonChunkHeader.chunkSize);
    }();

    ScopedObject<JSON::Document> document = JSON::Document::Parse(jsonData).Unwrap();

    // ...

    return new Model();
}

}
