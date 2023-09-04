// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/DataManagement/Result.hh>
#include <Cell/System/String.hh>

namespace Cell::DataManagement::Foreign {

// Decodes a base64 string into its raw bytes.
Wrapped<uint8_t*, Result> Base64Decode(const System::String& data);

// Encodes raw bytes into a base64 string.
Wrapped<System::String, Result> Base64Encode(const uint8_t* CELL_NONNULL data);

}
