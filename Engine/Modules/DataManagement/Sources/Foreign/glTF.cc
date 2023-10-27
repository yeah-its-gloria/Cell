// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/DataManagement/Foreign/JSON.hh>
#include <Cell/DataManagement/Foreign/glTF.hh>
#include <Cell/System/Panic.hh>

namespace Cell::DataManagement::Foreign {

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

Result ParseGlTF(const uint8_t* data, const size_t size) {
    if (size == 0) {
        return Result::InvalidParameters;
    }

    glTFHeader* header = (glTFHeader*)(data);
    CELL_ASSERT(header->magic == glTFHeaderMagic && header->version == 2 && header->size == size);

    glTFChunkHeader* jsonChunkHeader = (glTFChunkHeader*)(data + sizeof(glTFHeader));
    CELL_ASSERT(jsonChunkHeader->chunkSize > 0 && jsonChunkHeader->chunkSize < size - sizeof(glTFHeader) && jsonChunkHeader->chunkType == glTFChunkType::JSON);

    System::String jsonData((const char*)data + sizeof(glTFHeader) + sizeof(glTFChunkHeader), jsonChunkHeader->chunkSize * sizeof(char));

    Collection::List<JSON::Value> values = JSON::Parse(jsonData).Unwrap();
    (void)(values);

    // ...

    CELL_DEBUGBREAK;

    return Result::Success;
}

}
