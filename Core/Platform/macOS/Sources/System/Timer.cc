// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Panic.hh>
#include <Cell/System/Timer.hh>

#include <time.h>
#include <unistd.h>

namespace Cell::System {

uint64_t GetPreciseTickerValue() {
    const uint64_t result = clock_gettime_nsec_np(CLOCK_MONOTONIC);
    return result / 1000;
}

void Sleep(const uint32_t milliseconds) {
    SleepPrecise(milliseconds * 1000);
}

void SleepPrecise(const uint64_t microseconds) {
    const struct timespec timeout = {
        .tv_sec  = (time_t)(microseconds / 1000000),
        .tv_nsec = (time_t)(microseconds % 1000000)
    };

    /*const int result =*/ nanosleep(&timeout, nullptr);
    //CELL_ASSERT(result == 0);
}

}
