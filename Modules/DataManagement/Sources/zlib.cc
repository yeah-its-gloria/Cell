// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/DataManagement/zlib.hh>
#include <Cell/System/Log.hh>

#include <zlib-ng.h>

namespace Cell::DataManagement {

Wrapped<uint8_t*, Result> zlibDecompress(const Memory::IBlock& input, const size_t outSize) {
    if (input.GetSize() < 7 || outSize < input.GetSize()) {
        return Result::InvalidSize;
    }

    uint8_t* output = Memory::Allocate<uint8_t>(outSize);

    size_t capacity = outSize;
    const int32_t result = zng_uncompress(output, &capacity, input.AsBytes(), input.GetSize());
    switch (result) {
    case Z_OK: {
        break;
    }

    case Z_MEM_ERROR: {
        Memory::Free(output);
        return Result::NotEnoughMemory;
    }

    case Z_BUF_ERROR: {
        Memory::Free(output);
        return Result::NoSpaceInBuffer;
    }

    case Z_DATA_ERROR: {
        Memory::Free(output);
        return Result::InvalidData;
    }

    default: {
        System::Panic("zng_uncompress failed");
    }
    }

    CELL_ASSERT(capacity <= outSize);

    return output;
}

}
