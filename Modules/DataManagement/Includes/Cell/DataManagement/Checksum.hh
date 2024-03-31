// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Memory/Block.hh>

namespace Cell::DataManagement {

// Calculates the CRC32 value for the given block of data.
CELL_FUNCTION uint32_t CRC32Calculate(const Memory::IBlock& block);

// Calculates the ADLER32 value for the given block of data.
CELL_FUNCTION uint32_t ADLER32Calculate(const Memory::IBlock& block);

}
