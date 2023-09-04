// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/DataManagement/Result.hh>

namespace Cell::DataManagement::Foreign {

// Parses an HTTP response.
CELL_FUNCTION Result HTTPParseResponse(const uint8_t* CELL_NONNULL data, const size_t size);

}
