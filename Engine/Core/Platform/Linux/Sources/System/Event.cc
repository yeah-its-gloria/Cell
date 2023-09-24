// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Event.hh>
#include <Cell/System/Memory.hh>
#include <Cell/System/Panic.hh>

#include <errno.h>
#include <semaphore.h>

namespace Cell::System {

Event::Event(const bool createSignaled) {
    sem_t* semaphore = System::AllocateMemory<sem_t>();

    const int result = sem_init(semaphore, 0, 0);
    CELL_ASSERT(result == 0);

    this->handle = (uintptr_t)semaphore;

    if (createSignaled) {
        this->Signal();
    }
}

Event::~Event() {
    sem_t* semaphore = (sem_t*)this->handle;

    sem_destroy(semaphore);
    System::FreeMemory(semaphore);
}

void Event::Signal() {
    sem_post((sem_t*)this->handle);
}

void Event::Reset() {
    sem_t* semaphore = (sem_t*)this->handle;

    int result;
    int value = 0;
    while (true) {
        result = sem_getvalue(semaphore, &value);
        CELL_ASSERT(result == 0);
        if (value == 0) {
            break;
        }

        result = sem_wait(semaphore);
        CELL_ASSERT(result == 0);
    }
}

EventWaitResult Event::Wait(const uint32_t timeoutMs) {
    int result;

    if (timeoutMs > 0) {
        const struct timespec timeout =
            {
                .tv_sec  =  timeoutMs / 1000,
                .tv_nsec = (timeoutMs % 1000) * 1000
            };

        result = sem_timedwait((sem_t*)this->handle, &timeout);

    } else {
        result = sem_wait((sem_t*)this->handle);
    }

    if (result == 0) {
        this->Signal(); // sem_wait/sem_timedwait decrement the counter, which is not the behavior we want (thanks Linux...)
        return EventWaitResult::Signaled;
    } else if (errno == ETIMEDOUT) {
        return EventWaitResult::Timeout;
    }

    System::Panic("sem_wait/sem_timedwait failed");
}

}
