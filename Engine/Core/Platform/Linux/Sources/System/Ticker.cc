// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Panic.hh>
#include <Cell/System/Ticker.hh>

#include <time.h>

namespace Cell::System {

uint64_t GetPreciseTickerValue() {
    timespec timespec { };
    const int result = clock_gettime(CLOCK_MONOTONIC, &timespec);
    CELL_ASSERT(result == 0);

    return timespec.tv_nsec / 1000 + timespec.tv_sec * 1000000;
}

}
