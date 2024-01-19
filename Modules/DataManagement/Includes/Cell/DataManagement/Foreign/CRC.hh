// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::DataManagement::Foreign {

CELL_FUNCTION uint32_t GenerateCRC(const uint8_t* data, const size_t size);

}
