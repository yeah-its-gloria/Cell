// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Memory/Allocator.hh>
#include <Cell/System/Mutex.hh>
#include <Cell/System/Panic.hh>

#include <pthread.h>

namespace Cell::System {

Mutex::Mutex(const bool createLocked) {
    pthread_mutex_t* mutex = Memory::Allocate<pthread_mutex_t>();
    const int result = pthread_mutex_init(mutex, nullptr);
    CELL_ASSERT(result == 0);

    this->impl = (uintptr_t)mutex;

    if (createLocked) {
        this->Lock();
    }
}

Mutex::~Mutex() {
    pthread_mutex_t* mutex = (pthread_mutex_t*)this->impl;

    pthread_mutex_destroy(mutex);
    Memory::Free(mutex);
}

void Mutex::Lock() {
    const int result = pthread_mutex_lock((pthread_mutex_t*)this->impl);
    CELL_ASSERT(result == 0);
}

void Mutex::Unlock() {
    const int result = pthread_mutex_unlock((pthread_mutex_t*)this->impl);
    CELL_ASSERT(result == 0);
}

}
