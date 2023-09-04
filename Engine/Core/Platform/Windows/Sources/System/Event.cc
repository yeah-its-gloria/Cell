// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Event.hh>
#include <Cell/System/Panic.hh>
#include <Cell/System/Platform/Windows/Includes.h>

namespace Cell::System {

Event::Event(const bool createSignaled) {
    HANDLE event = CreateEventW(nullptr, true, createSignaled, nullptr);
    CELL_ASSERT(event != nullptr);

    this->handle = (uintptr_t)event;
}

Event::~Event() {
    const BOOL result = CloseHandle((HANDLE)this->handle);
    CELL_ASSERT(result);
}

void Event::Signal() {
    const BOOL result = SetEvent((HANDLE)this->handle);
    CELL_ASSERT(result);
}

void Event::Reset() {
    const BOOL result = ResetEvent((HANDLE)this->handle);
    CELL_ASSERT(result);
}

EventWaitState Event::Wait(const uint32_t timeoutMs) {
    const DWORD result = WaitForSingleObjectEx((HANDLE)this->handle, timeoutMs == 0 ? INFINITE : timeoutMs, FALSE);
    switch (result) {
    case WAIT_OBJECT_0: {
        this->Signal();
        return EventWaitState::Signaled;
    }

    case WAIT_TIMEOUT: {
        return EventWaitState::Timeout;
    }

    default: {
        System::Panic("WaitForSingleObjectEx for event failed");
    }
    }
}

}
