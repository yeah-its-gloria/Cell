// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/DataManagement/Checksum.hh>

#include <zlib-ng.h>

namespace Cell::DataManagement {

uint32_t CRC32Calculate(const Memory::IBlock& block) {
    return zng_crc32_z(0, block.AsBytes(), block.GetSize());
}

uint32_t ADLER32Calculate(const Memory::IBlock& block) {
    return zng_adler32_z(0, block.AsBytes(), block.GetSize());
}

}
