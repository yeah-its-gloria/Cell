// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Memory/Allocator.hh>
#include <Cell/System/Event.hh>
#include <Cell/System/Panic.hh>

#include <dispatch/dispatch.h>

namespace Cell::System {

Event::Event(const bool createSignaled) {
    dispatch_semaphore_t semaphore = dispatch_semaphore_create(0);
    if (semaphore == nullptr) {
        System::Panic("dispatch_semaphore_create failed");
    }

    this->impl = (uintptr_t)semaphore;

    if (createSignaled) {
        this->Signal();
    }
}

Event::~Event() {
    // TODO: implement
}

void Event::Signal() {
    dispatch_semaphore_signal((dispatch_semaphore_t)this->impl);
}

void Event::Reset() {
    while (true) {
        const intptr_t result = dispatch_semaphore_wait((dispatch_semaphore_t)this->impl, DISPATCH_TIME_NOW);
        if (result != 0) {
            break;
        }
    }
}

EventWaitResult Event::Wait(const uint32_t timeoutMs) {
    dispatch_time_t timeout = DISPATCH_TIME_FOREVER;
    if (timeoutMs != 0) {
        timeout = dispatch_time(DISPATCH_TIME_NOW, timeoutMs * (NSEC_PER_SEC / MSEC_PER_SEC));
    }

    const intptr_t result = dispatch_semaphore_wait((dispatch_semaphore_t)this->impl, timeout);
    if (result == 0) {
        return EventWaitResult::Signaled;
    }

    return EventWaitResult::Timeout;
}

}
