// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/String.hh>
#include <Cell/DataManagement/Result.hh>
#include <Cell/Memory/Block.hh>

namespace Cell::DataManagement {

// Decompresses 1 zlib formatted block into the given output storage.
// Output block must have the needed space for the decompressed data.
CELL_FUNCTION Wrapped<uint8_t*, Result> zlibDecompress(const Memory::IBlock& input, const size_t outSize);

}
