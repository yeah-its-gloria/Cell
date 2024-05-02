// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Memory/Allocator.hh>
#include <Cell/System/Event.hh>
#include <Cell/System/Panic.hh>

#include <mach/mach_init.h>
#include <mach/semaphore.h>
#include <mach/task.h>

namespace Cell::System {

struct EventInfo {
    bool state;
    semaphore_t signal;
};

Event::Event(const bool createSignaled) {
    EventInfo* info = Memory::Allocate<EventInfo>();

    info->state = createSignaled;
    semaphore_create(mach_task_self(), &info->signal, SYNC_POLICY_FIFO, 0);

    this->impl = (uintptr_t)info;
}

Event::~Event() {
    EventInfo* info = (EventInfo*)this->impl;

    const kern_return_t result = semaphore_destroy(mach_task_self(), info->signal);
    CELL_ASSERT(result == KERN_SUCCESS);

    Memory::Free(info);
}

void Event::Signal() {
    EventInfo* info = (EventInfo*)this->impl;
    if (info->state) {
        return;
    }

    info->state = true;

    const kern_return_t result = semaphore_signal_all(info->signal);
    CELL_ASSERT(result == KERN_SUCCESS);
}

void Event::Reset() {
    EventInfo* info = (EventInfo*)this->impl;
    info->state = false;
}

Result Event::Wait(const uint32_t timeoutMs) {
    EventInfo* info = (EventInfo*)this->impl;

    if (info->state) {
        return Result::Success;
    }

    if (timeoutMs == 0) {
        const kern_return_t result = semaphore_wait(info->signal);
        CELL_ASSERT(result == KERN_SUCCESS && info->state);

        return Result::Success;
    }

    const mach_timespec_t timeout = {
        .tv_sec  = (timeoutMs / 1000),
        .tv_nsec = (clock_res_t)((timeoutMs % 1000) * 1000000)
    };

    const kern_return_t result = semaphore_timedwait(info->signal, timeout);
    switch (result) {
    case KERN_SUCCESS: {
        CELL_ASSERT(info->state);
        return Result::Success;
    }

    case KERN_OPERATION_TIMED_OUT: {
        return Result::Timeout;
    }

    default: {
        System::Panic("semaphore_timedwait failed");
    }
    }
}

}
