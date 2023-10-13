// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::System {

// Returns a precise timer value in microseconds.
CELL_FUNCTION uint64_t GetPreciseTickerValue();

// Sleeps for the given amount of milliseconds.
CELL_FUNCTION void Sleep(const uint32_t milliseconds);

// Sleeps for the given amount of microseconds.
CELL_FUNCTION void SleepPrecise(const uint64_t microseconds);

}
