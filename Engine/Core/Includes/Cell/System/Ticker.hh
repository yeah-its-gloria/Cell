// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::System {

// Returns a precise timer value in microseconds.
CELL_FUNCTION uint64_t GetPreciseTickerValue();

}
