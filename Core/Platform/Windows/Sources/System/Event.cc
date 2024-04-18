// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Event.hh>
#include <Cell/System/Panic.hh>
#include <Cell/System/Platform/Windows/Includes.h>

namespace Cell::System {

Event::Event(const bool createSignaled) {
    HANDLE event = CreateEventW(nullptr, TRUE, createSignaled ? TRUE : FALSE, nullptr);
    CELL_ASSERT(event != nullptr);

    this->impl = (uintptr_t)event;
}

Event::~Event() {
    const BOOL result = CloseHandle((HANDLE)this->impl);
    CELL_ASSERT(result == TRUE);
}

void Event::Signal() {
    const BOOL result = SetEvent((HANDLE)this->impl);
    CELL_ASSERT(result == TRUE);
}

void Event::Reset() {
    const BOOL result = ResetEvent((HANDLE)this->impl);
    CELL_ASSERT(result == TRUE);
}

Result Event::Wait(const uint32_t timeoutMs) {
    const DWORD result = WaitForSingleObjectEx((HANDLE)this->impl, timeoutMs == 0 ? INFINITE : timeoutMs, FALSE);
    switch (result) {
    case WAIT_OBJECT_0: {
        this->Signal();
        return Result::Success;
    }

    case WAIT_TIMEOUT: {
        return Result::Timeout;
    }

    default: {
        System::Panic("WaitForSingleObjectEx for event failed");
    }
    }
}

}
