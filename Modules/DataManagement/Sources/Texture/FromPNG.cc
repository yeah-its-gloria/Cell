// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/DataManagement/Checksum.hh>
#include <Cell/DataManagement/Texture.hh>
#include <Cell/DataManagement/zlib.hh>
#include <Cell/Memory/UnownedBlock.hh>
#include <Cell/System/Log.hh>
#include <Cell/Utilities/Byteswap.hh>
#include <Cell/Utilities/Preprocessor.hh>
#include <Cell/Utilities/Reader.hh>

#include <math.h>

namespace Cell::DataManagement {

#define U64_SHIFT(v, b) (((uint64_t)(v)) << b)

struct CELL_PACKED(1) ChunkHeader {
    uint32_t size;
    uint32_t identifier;
};

enum class IHDRColorType : uint8_t {
    Grayscale      = 0,
    TrueColor      = 2,
    Indexed        = 3,
    GrayscaleAlpha = 4,
    TrueColorAlpha = 6
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

const uint64_t PNGMagic = U64_SHIFT(0x89,  0) |
                          U64_SHIFT('P',   8) |
                          U64_SHIFT('N',  16) |
                          U64_SHIFT('G',  24) |
                          U64_SHIFT('\r', 32) |
                          U64_SHIFT('\n', 40) |
                          U64_SHIFT(0x1a, 48) |
                          U64_SHIFT('\n', 56);

// Implemented

const uint32_t IHDRIdentifier = 'I' | 'H' << 8 | 'D' << 16 | 'R' << 24;
const uint32_t IDATIdentifier = 'I' | 'D' << 8 | 'A' << 16 | 'T' << 24;
const uint32_t PLTEIdentifier = 'P' | 'L' << 8 | 'T' << 16 | 'E' << 24;
const uint32_t IENDIdentifier = 'I' | 'E' << 8 | 'N' << 16 | 'D' << 24;

// Aware

const uint32_t sRGBIdentifier = 's' | 'R' << 8 | 'G' << 16 | 'B' << 24;
const uint32_t gAMAIdentifier = 'g' | 'A' << 8 | 'M' << 16 | 'A' << 24;
const uint32_t pHYsIdentifier = 'p' | 'H' << 8 | 'Y' << 16 | 's' << 24;

CELL_FUNCTION_INTERNAL uint32_t GenerateCRCForChunk(const uint32_t& identifier, const uint8_t* data, const size_t& size) {
    uint8_t* buffer = Memory::Allocate<uint8_t>(sizeof(uint32_t) + size);

    *(uint32_t*)buffer = identifier;
    Memory::Copy<uint8_t>(buffer + sizeof(uint32_t), data, size);

    const uint32_t crc = CRC32Calculate(Memory::UnownedBlock<uint8_t> { buffer, 4 + size });

    Memory::Free(buffer);
    return Utilities::ByteswapFromBE(crc);
}

CELL_FUNCTION_INTERNAL bool IsValidDepthForColorType(const uint8_t& depth, const IHDRColorType& type) {
    switch (type) {
    case IHDRColorType::Grayscale: {
        return depth == 1 || depth == 2 || depth == 4 || depth == 8 || depth == 16;
    }

    case IHDRColorType::Indexed: {
        return depth == 8;
    }

    case IHDRColorType::TrueColor:
    case IHDRColorType::GrayscaleAlpha:
    case IHDRColorType::TrueColorAlpha: {
        return depth == 8 || depth == 16;
    }

    default: {
        return false;
    }
    }
}

CELL_FUNCTION_INTERNAL uint8_t PerformFilter(const uint8_t& filter, const uint8_t& current, const uint8_t previous, const uint8_t previousScanline, const uint8_t previousScanlinePrevious) {
    switch (filter) {
    // None
    case 0: {
        return current;
    }

    // Sub
    case 1: {
        return current + previous;
    }

    // Up
    case 2: {
        return current + previousScanline;
    }

    // Average
    case 3: {
        return current + (floor(previous + previousScanline) / 2.0);
    }

    // Paeth
    case 4: {
        const uint8_t value = [&] {
            const uint16_t p = previous + previousScanline - previousScanlinePrevious;

            const uint16_t pa = abs(p - previous);
            const uint16_t pb = abs(p - previousScanline);
            const uint16_t pc = abs(p - previousScanlinePrevious);

            if (pa <= pb && pa <= pc) {
                return previous;
            }

            if (pb <= pc) {
                return previousScanline;
            }

            return previousScanlinePrevious;
        }();

        return current + value;
    }

    // unknown filter
    default: {
        break;
    }
    }

    return current;
}

Wrapped<Texture*, Result> Texture::FromPNG(const Memory::IBlock& block) {
    Utilities::Reader reader(block);

    // magic check

    uint64_t magic = reader.Read<uint64_t>();
    if (magic != PNGMagic) {
        return Result::InvalidSignature;
    }

    // IHDR load

    ChunkHeader chunkHeader = reader.Read<ChunkHeader>();
    chunkHeader.size = Utilities::ByteswapFromBE(chunkHeader.size);

    if (chunkHeader.size != sizeof(IHDR)) {
        return Result::InvalidSize;
    } else if (chunkHeader.identifier != IHDRIdentifier) {
        return Result::InvalidIdentifier;
    }

    IHDR header = reader.Read<IHDR>();

    const uint32_t crc = reader.Read<uint32_t>();
    if (crc != GenerateCRCForChunk(IHDRIdentifier, (uint8_t*)&header, sizeof(IHDR))) {
        return Result::InvalidChecksum;
    }

    header.width = Utilities::ByteswapFromBE(header.width);
    header.height = Utilities::ByteswapFromBE(header.height);

    if (header.width == 0 || header.height == 0 || header.filterMethod != 0 || header.compressionMethod != 0 ||
        !IsValidDepthForColorType(header.bitDepth, header.colorType)) {
        return Result::InvalidData;
    }

    const bool needsPalette = header.colorType == IHDRColorType::Indexed;

    // TODO: chunk loading

    uint8_t* imageData = nullptr;
    size_t imageDataSize = 0;

    uint8_t* paletteData = nullptr;
    size_t paletteDataSize = 0;

    while (true) {
        CELL_ASSERT(reader.GetCurrentOffset() < block.GetSize());

        chunkHeader = reader.Read<ChunkHeader>();
        chunkHeader.size = Utilities::ByteswapFromBE(chunkHeader.size);

        if (chunkHeader.identifier == IENDIdentifier) {
            break;
        }

        switch (chunkHeader.identifier) {
        case IDATIdentifier: {
            // TODO: allow multiple data chunks
            CELL_ASSERT(imageData == nullptr);

            imageDataSize = chunkHeader.size;

            imageData = Memory::Allocate<uint8_t>(imageDataSize);
            reader.ReadBytes(imageData, imageDataSize, false);
            break;
        }

        case PLTEIdentifier: {
            CELL_ASSERT(needsPalette && paletteData == nullptr);

            if (chunkHeader.size % 3 != 0) {
                if (imageData != nullptr) {
                    Memory::Free(imageData);
                }

                return Result::InvalidSize;
            }

            paletteDataSize = chunkHeader.size;

            paletteData = Memory::Allocate<uint8_t>(paletteDataSize);
            reader.ReadBytes(paletteData, paletteDataSize, false);
            break;
        }

        case sRGBIdentifier:
        case gAMAIdentifier:
        case pHYsIdentifier: {
            // unused

            break;
        }

        // skip unknown chunks, log presence
        default: {
            System::Log("Unknown chunk: %: %", String((char*)&chunkHeader.identifier, 4), chunkHeader.size);
            break;
        }
        }

        CELL_ASSERT(chunkHeader.size > 0);
        uint8_t* chunkData = Memory::Allocate<uint8_t>(chunkHeader.size);
        reader.ReadBytes(chunkData, chunkHeader.size);

        const uint32_t crc = reader.Read<uint32_t>();
        const uint32_t crcGen = GenerateCRCForChunk(chunkHeader.identifier, chunkData, chunkHeader.size);
        Memory::Free(chunkData);

        if (crc != crcGen) {
            if (imageData != nullptr) {
                Memory::Free(imageData);
            }

            if (paletteData != nullptr) {
                Memory::Free(paletteData);
            }

            return Result::InvalidChecksum;
        }
    }

    CELL_ASSERT(imageData != nullptr);
    if (needsPalette) {
        CELL_ASSERT(paletteData != nullptr);
    }

    // DECOMPRESSION

    const uint8_t bytesPerPixel = [&] {
        switch (header.colorType) {
        case IHDRColorType::Indexed: {
            return 1;
        }

        case IHDRColorType::TrueColorAlpha: {
            return 4;
        }

        default: {
            CELL_UNIMPLEMENTED
        }
        }
    }();

    const size_t decompressedSize = (header.width + 1) * header.height * ((header.bitDepth / 8) * bytesPerPixel);

    Wrapped<uint8_t*, Result> zlibResult = zlibDecompress(Memory::UnownedBlock { imageData, imageDataSize }, decompressedSize);
    if (!zlibResult.IsValid()) {
        return zlibResult.Result();
    }

    ScopedBlock<uint8_t> decompressedBlock = zlibResult.Unwrap();

    // DECODE

    // TODO: the entire filter system is *completely broken* right now

    uint32_t* rgba = Memory::Allocate<uint32_t>(header.width * header.height);

    size_t pixelIndex = 0;
    for (size_t y = 0; y < header.height; y++) {
        const uint8_t filter = decompressedBlock[y * header.height];
        switch (header.colorType) {
        case IHDRColorType::Indexed: {
            CELL_ASSERT(needsPalette);

            for (size_t x = 0; x < header.width; x++) {
                const size_t curIndex = x + (y * header.height);

                rgba[pixelIndex++] = [&] {
                    const size_t index = PerformFilter(filter,
                                                       decompressedBlock[curIndex],
                                                       x > 0 ? decompressedBlock[curIndex] : 0,
                                                       x > 0 && y > 0 ? decompressedBlock[curIndex - header.height + 1] : 0,
                                                       x > 1 && y > 0 ? decompressedBlock[curIndex - header.height] : 0) + 1;

                    return paletteData[decompressedBlock[index] + 0] << 24 |
                           paletteData[decompressedBlock[index] + 1] << 16 |
                           paletteData[decompressedBlock[index] + 2] <<  8 |
                           0xff;
                }();
            }

            break;
        }

        case IHDRColorType::TrueColorAlpha: {
            for (size_t x = 0; x < header.width; x++) {
                const size_t index = (x * 4) + (y * header.height) + 1;

                const uint8_t R = PerformFilter(filter,
                                                decompressedBlock[index],
                                                x > 0 ? decompressedBlock[index - 1] : 0,
                                                x > 0 && y > 0 ? decompressedBlock[index - header.height] : 0,
                                                x > 1 && y > 0 ? decompressedBlock[index - header.height - 1] : 0);

                const uint8_t G = PerformFilter(filter,
                                                decompressedBlock[index + 1],
                                                x > 0 ? decompressedBlock[index] : 0,
                                                x > 0 && y > 0 ? decompressedBlock[index - header.height + 1] : 0,
                                                x > 1 && y > 0 ? decompressedBlock[index - header.height] : 0);

                const uint8_t B = PerformFilter(filter,
                                                decompressedBlock[index + 2],
                                                x > 0 ? decompressedBlock[index + 1] : 0,
                                                x > 0 && y > 0 ? decompressedBlock[index - header.height + 2] : 0,
                                                x > 1 && y > 0 ? decompressedBlock[index - header.height + 1] : 0);

                const uint8_t A = PerformFilter(filter,
                                                decompressedBlock[index + 3],
                                                x > 0 ? decompressedBlock[index + 2] : 0,
                                                x > 0 && y > 0 ? decompressedBlock[index - header.height + 3] : 0,
                                                x > 1 && y > 0 ? decompressedBlock[index - header.height + 2] : 0);

                rgba[pixelIndex++] = R | B << 8 | G << 16 | A << 24;
            }

            break;
        }

        default: {
            CELL_UNIMPLEMENTED
        }
        }
    }

    // CLEANUP

    if (needsPalette) {
        Memory::Free(paletteData);
    }

    Memory::Free(imageData);

    return new Texture(header.width, header.height, 1, rgba);
}

}
