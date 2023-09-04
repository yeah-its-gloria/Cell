// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "CRC.hh"

#include <Cell/DataManagement/Foreign/PNG.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Memory.hh>
#include <Cell/System/Panic.hh>
#include <Cell/Utilities/Byteswap.hh>

namespace Cell::DataManagement::Foreign {

struct CELL_PACKED(1) ChunkHeader {
    uint32_t size;
    uint8_t identifier[4];
};

enum class IHDRColorType : uint8_t {
    Grayscale = 0,
    TrueColor = 2,
    Indexed = 3,
    GrayscaleWithAlpha = 4,
    TrueColorWithAlpha = 6
};

struct CELL_PACKED(1) IHDR {
    uint32_t width;
    uint32_t height;
    uint8_t bitDepth;
    IHDRColorType colorType;
    uint8_t compressionMethod;
    uint8_t filterMethod;
    uint8_t interlaceMethod;
};

const uint8_t PNGMagic[8] = {0x89, 'P', 'N', 'G', '\r', '\n', 0x1a, '\n'};

// Implemented

const uint8_t IHDRIdentifier[4] = {'I', 'H', 'D', 'R'};
const uint8_t IDATIdentifier[4] = {'I', 'D', 'A', 'T'};
const uint8_t PLTEIdentifier[4] = {'P', 'L', 'T', 'E'};
const uint8_t IENDIdentifier[4] = {'I', 'E', 'N', 'D'};

// Aware

const uint8_t sRGBIdentifier[4] = {'s', 'R', 'G', 'B'};
const uint8_t gAMAIdentifier[4] = {'g', 'A', 'M', 'A'};
const uint8_t pHYsIdentifier[4] = {'p', 'H', 'Y', 's'};

// Generates a network order CRC for the given chunk ID and data.
CELL_FUNCTION_INTERNAL uint32_t GenerateCRCForChunk(const uint8_t id[4], const uint8_t* data, const size_t size) {
    uint8_t* buffer = System::AllocateMemory<uint8_t>(4 + size);

    System::CopyMemory(buffer, id, 4);
    System::CopyMemory(buffer + 4, data, size);

    const uint32_t crc = GenerateCRC(buffer, 4 + size);

    System::FreeMemory(buffer);

    return Utilities::Byteswap(crc);
}

Wrapped<PNG*, Result> PNG::Decode(const uint8_t* data, const size_t size) {
    CELL_ASSERT(size > 8);

    size_t offset = 0;
    if (!System::CompareMemory<uint8_t>(data + offset, PNGMagic, sizeof(PNGMagic))) {
        return Result::InvalidSignature;
    }

    offset += sizeof(PNGMagic);

    ChunkHeader chunkHeader {};
    System::CopyMemory<ChunkHeader>(&chunkHeader, (ChunkHeader*)(data + offset));
    offset += sizeof(ChunkHeader);

    chunkHeader.size = Utilities::Byteswap(chunkHeader.size);

    if (chunkHeader.size != sizeof(IHDR)) {
        return Result::InvalidSize;
    } else if (!System::CompareMemory(chunkHeader.identifier, IHDRIdentifier, 4)) {
        return Result::InvalidIdentifier;
    }

    IHDR header {};
    System::CopyMemory<IHDR>(&header, (IHDR*)(data + offset));
    offset += sizeof(IHDR);

    uint32_t crc = *((uint32_t*)(data + offset));
    offset += sizeof(uint32_t);

    if (crc != GenerateCRCForChunk(IHDRIdentifier, (uint8_t*)&header, sizeof(IHDR))) {
        return Result::InvalidChecksum;
    }

    header.width = Utilities::ByteswapBEPlatform(header.width);
    header.height = Utilities::ByteswapBEPlatform(header.height);

    if (header.filterMethod != 0 || header.compressionMethod != 0) {
        return Result::InvalidData;
    }

    const bool needsPalette = header.colorType == IHDRColorType::Indexed;

    // TODO: chunk loading

    uint8_t* imageData = nullptr;
    uint8_t* paletteData = nullptr;

    while (offset < size) {
        System::CopyMemory<ChunkHeader>(&chunkHeader, (ChunkHeader*)(data + offset));
        offset += sizeof(ChunkHeader);

        chunkHeader.size = Utilities::Byteswap(chunkHeader.size);

        if (System::CompareMemory(chunkHeader.identifier, IENDIdentifier, 4)) {
            break;
        } else if (System::CompareMemory(chunkHeader.identifier, IDATIdentifier, 4)) {
            CELL_ASSERT(imageData == nullptr);// TODO: allow multiple IDAT chunks

            imageData = System::AllocateMemory<uint8_t>(chunkHeader.size);
            System::CopyMemory(imageData, data + offset, chunkHeader.size);
        } else if (System::CompareMemory(chunkHeader.identifier, PLTEIdentifier, 4)) {
            CELL_ASSERT(needsPalette);

            if (chunkHeader.size % 3 != 0) {
                if (imageData != nullptr) {
                    System::FreeMemory(imageData);
                }

                return Result::InvalidSize;
            }

            paletteData = System::AllocateMemory<uint8_t>(chunkHeader.size);
            System::CopyMemory(paletteData, data + offset, chunkHeader.size);
        } else if (!System::CompareMemory(chunkHeader.identifier, sRGBIdentifier, 4) &&
                   !System::CompareMemory(chunkHeader.identifier, gAMAIdentifier, 4) &&
                   !System::CompareMemory(chunkHeader.identifier, pHYsIdentifier, 4)) {
            System::Log("Unknown chunk: %c%c%c%c: %d", chunkHeader.identifier[0], chunkHeader.identifier[1], chunkHeader.identifier[2], chunkHeader.identifier[3], chunkHeader.size);
        }

        CELL_ASSERT(chunkHeader.size > 0);
        uint8_t* chunkData = System::AllocateMemory<uint8_t>(chunkHeader.size);

        System::CopyMemory(chunkData, data + offset, chunkHeader.size);
        offset += chunkHeader.size;

        crc = *((uint32_t*)(data + offset));
        offset += sizeof(uint32_t);

        const uint32_t crcGen = GenerateCRCForChunk(chunkHeader.identifier, chunkData, chunkHeader.size);
        System::FreeMemory(chunkData);

        if (crc != crcGen) {
            if (imageData != nullptr) {
                System::FreeMemory(imageData);
            }

            if (paletteData != nullptr) {
                System::FreeMemory(paletteData);
            }

            return Result::InvalidChecksum;
        }
    }

    CELL_ASSERT(imageData != nullptr);
    if (needsPalette) {
        CELL_ASSERT(paletteData != nullptr);
    }

    // TODO: implement zlib, DEFLATE and Adler32

    if (needsPalette) {
        System::FreeMemory(paletteData);
    }

    return new PNG(header.width, header.height, header.bitDepth, imageData);
}

PNG::~PNG() {
    (void)(this->width);
    (void)(this->height);
    (void)(this->depth);

    System::FreeMemory(this->data);
}

}
