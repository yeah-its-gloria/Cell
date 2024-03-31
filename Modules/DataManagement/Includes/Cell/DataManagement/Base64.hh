// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/String.hh>
#include <Cell/DataManagement/Result.hh>
#include <Cell/Memory/Block.hh>

namespace Cell::DataManagement::Foreign {

// Decodes a base64 string into its raw bytes.
CELL_FUNCTION Wrapped<uint8_t*, Result> Base64Decode(const String& data);

// Encodes raw bytes into a base64 string.
CELL_FUNCTION Wrapped<String, Result> Base64Encode(const Memory::IBlock& data);

}
