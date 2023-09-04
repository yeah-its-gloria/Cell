// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Panic.hh>
#include <Cell/System/Sleep.hh>
#include <Cell/System/Platform/Windows/Includes.h>

namespace Cell::System {

void Sleep(const uint32_t milliseconds) {
    ::Sleep(milliseconds);
}

void SleepPrecise(const uint64_t nanoseconds) {
    HANDLE timer = CreateWaitableTimerExW(nullptr, nullptr, CREATE_WAITABLE_TIMER_MANUAL_RESET | CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, TIMER_ALL_ACCESS);
    CELL_ASSERT(timer != nullptr);

    BOOL result = SetWaitableTimer(timer, (LARGE_INTEGER*)&nanoseconds, 0, nullptr, nullptr, FALSE);
    CELL_ASSERT(result);

    const DWORD wait_result = WaitForSingleObjectEx(timer, INFINITE, FALSE);
    CELL_ASSERT(wait_result == WAIT_OBJECT_0);

    result = CloseHandle(timer);
    CELL_ASSERT(result);
}

}
