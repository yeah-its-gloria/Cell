// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Memory/Allocator.hh>
#include <Cell/System/Event.hh>
#include <Cell/System/Panic.hh>

#include <errno.h>
#include <semaphore.h>

namespace Cell::System {

Event::Event(const bool createSignaled) {
    sem_t* semaphore = Memory::Allocate<sem_t>();

    const int result = sem_init(semaphore, 0, 0);
    CELL_ASSERT(result == 0);

    this->impl = (uintptr_t)semaphore;

    if (createSignaled) {
        this->Signal();
    }
}

Event::~Event() {
    sem_t* semaphore = (sem_t*)this->impl;

    sem_destroy(semaphore);
    Memory::Free(semaphore);
}

void Event::Signal() {
    sem_post((sem_t*)this->impl);
}

void Event::Reset() {
    sem_t* semaphore = (sem_t*)this->impl;

    int value = 0;
    while (true) {
        int result = sem_getvalue(semaphore, &value);
        CELL_ASSERT(result == 0);
        if (value == 0) {
            break;
        }

        result = sem_wait(semaphore);
        CELL_ASSERT(result == 0);
    }
}

Result Event::Wait(const uint32_t timeoutMs) {
    const bool success = [&] {
        if (timeoutMs == 0) {
            return sem_wait((sem_t*)this->impl) == 0;
        }

        const struct timespec timeout = {
            .tv_sec  =  timeoutMs / 1000,
            .tv_nsec = (timeoutMs % 1000) * 1000
        };

        return sem_timedwait((sem_t*)this->impl, &timeout) == 0;
    }();

    if (success) {
        this->Signal();
        return Result::Success;
    }

    if (errno == ETIMEDOUT) {
        return Result::Timeout;
    }

    System::Panic("sem_wait/sem_timedwait failed");
}

}
