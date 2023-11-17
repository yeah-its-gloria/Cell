// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/DataManagement/Result.hh>

namespace Cell::DataManagement::Foreign {

// Parses glTF data.
CELL_FUNCTION Result ParseGlTF(const uint8_t* CELL_NONNULL data, const size_t size);

}
