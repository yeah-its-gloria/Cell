// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Panic.hh>
#include <Cell/System/Ticker.hh>
#include <Cell/System/Platform/Windows/Includes.h>

namespace Cell::System {

static uint64_t tick_frequency_microseconds = 0;

uint64_t GetPreciseTickerValue() {
    if (tick_frequency_microseconds == 0) {
        const BOOL perf_result = QueryPerformanceFrequency((LARGE_INTEGER*)&tick_frequency_microseconds);
        CELL_ASSERT(perf_result == TRUE);

        tick_frequency_microseconds /= 1000000; // seconds -> microseconds

        CELL_ASSERT(tick_frequency_microseconds > 0);
    }

    uint64_t tick = 0;
    const BOOL result = QueryPerformanceCounter((LARGE_INTEGER*)&tick);
    CELL_ASSERT(result == TRUE);

    return tick / tick_frequency_microseconds;
}

}
