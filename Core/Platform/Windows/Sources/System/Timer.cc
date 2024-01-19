// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Panic.hh>
#include <Cell/System/Timer.hh>
#include <Cell/System/Platform/Windows/Includes.h>

CELL_UNMANGLED {
    int32_t NtDelayExecution(BOOL alertable, int64_t* delayInterval);
    int32_t NtQueryPerformanceCounter(int64_t* counter, int64_t* freq);
}

namespace Cell::System {

uint64_t GetPreciseTickerValue() {
    int64_t counter, freq = 0;

    const int32_t result = NtQueryPerformanceCounter(&counter, &freq);
    CELL_ASSERT(result == ERROR_SUCCESS);

    return (counter * 1000000) / freq; // sec -> usec, freq -> tick/Hz
}

void Sleep(const uint32_t milliseconds) {
    ::Sleep(milliseconds);
}

void SleepPrecise(const uint64_t microseconds) {
    CELL_ASSERT(microseconds <= INT64_MAX);

    int64_t converted = microseconds * 10; // expects 100 ns
    const int32_t result = NtDelayExecution(TRUE, &converted);
    CELL_ASSERT(result == ERROR_SUCCESS);
}

}
