// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Panic.hh>
#include <Cell/System/Timer.hh>

#include <time.h>
#include <unistd.h>

namespace Cell::System {

uint64_t GetPreciseTickerValue() {
    timespec timespec { };
    const int result = clock_gettime(CLOCK_MONOTONIC, &timespec);
    CELL_ASSERT(result == 0);

    return timespec.tv_nsec / 1000 + timespec.tv_sec * 1000000;
}

void Sleep(const uint32_t milliseconds) {
    const int result = usleep(milliseconds * 1000);
    CELL_ASSERT(result == 0);
}

void SleepPrecise(const uint64_t nanoseconds) {
    const struct timespec timeout = {
        .tv_sec  = (time_t)(nanoseconds / 1000000000),
        .tv_nsec = (time_t)(nanoseconds % 1000000000)
    };

    const int result = nanosleep(&timeout, nullptr);
    CELL_ASSERT(result == 0);
}

}
