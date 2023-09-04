// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Panic.hh>
#include <Cell/System/Sleep.hh>

#include <time.h>
#include <unistd.h>

namespace Cell::System {

void Sleep(const uint32_t milliseconds) {
    const int result = usleep(milliseconds * 1000);
    CELL_ASSERT(result == 0);
}

void SleepPrecise(const uint64_t nanoseconds) {
    const struct timespec timeout = {
        .tv_sec  = (__time_t)(nanoseconds / 1000000000),
        .tv_nsec = (__time_t)(nanoseconds % 1000000000)
    };

    const int result = nanosleep(&timeout, nullptr);
    CELL_ASSERT(result == 0);
}

}
