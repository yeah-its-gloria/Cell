// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Memory.hh>
#include <Cell/System/Mutex.hh>
#include <Cell/System/Panic.hh>

#include <pthread.h>

namespace Cell::System {

Mutex::Mutex(const bool createLocked) {
    pthread_mutex_t* mutex = System::AllocateMemory<pthread_mutex_t>();
    const int result = pthread_mutex_init(mutex, nullptr);
    CELL_ASSERT(result == 0);

    this->handle = (uintptr_t)mutex;

    if (createLocked) {
        this->Lock();
    }
}

Mutex::~Mutex() {
    pthread_mutex_t* mutex = (pthread_mutex_t*)this->handle;

    pthread_mutex_destroy(mutex);
    System::FreeMemory(mutex);
}

void Mutex::Lock() {
    const int result = pthread_mutex_lock((pthread_mutex_t*)this->handle);
    CELL_ASSERT(result == 0);
}

void Mutex::Unlock() {
    const int result = pthread_mutex_unlock((pthread_mutex_t*)this->handle);
    CELL_ASSERT(result == 0);
}

}
