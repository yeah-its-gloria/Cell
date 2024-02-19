// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/System/Event.hh>
#include <Cell/System/Panic.hh>
#include <Cell/System/Thread.hh>

#include <errno.h>
#include <pthread.h>
#include <signal.h>

namespace Cell::System {

struct ThreadData {
    Event* event;
    ThreadFunction function;
    void* parameter;
    uintptr_t* handle;
};

void* ThreadTrampoline(void* param) {
    struct ThreadData data = *(struct ThreadData*)param;
    data.event->Signal();
    data.function(data.parameter);
    *data.handle = 0;
    return nullptr;
}

Thread::Thread(ThreadFunction function, void* parameter, const String& name) {
    Event event;

    const ThreadData data = {
        .event     = &event,
        .function  = function,
        .parameter = parameter,
        .handle    = &this->handle
    };

    pthread_t thread = 0;
    const int result = pthread_create(&thread, nullptr, ThreadTrampoline, (void**)&data);
    CELL_ASSERT(result == 0);

    event.Wait();

    this->handle = (uintptr_t)thread;

    if (!name.IsEmpty()) {
        this->SetName(name);
    }
}


Thread::~Thread() {
    if (this->handle != 0) {
        pthread_kill((pthread_t)this->handle, SIGTERM);
    }
}

Result Thread::Join(const uint32_t timeoutMs) const {
    if (this->handle == 0) {
        return Result::Expired;
    }

    int result = 0;

    if (timeoutMs > 0) {
        struct timespec timeout = { .tv_sec = 0, .tv_nsec = timeoutMs * 1000 };
        if (timeoutMs >= 1000) {
            timeout.tv_sec = timeoutMs / 1000;
            timeout.tv_nsec = (timeoutMs % 1000) * 1000;
        }

        result = pthread_timedjoin_np((pthread_t)this->handle, nullptr, &timeout);
    } else {
        result = pthread_join((pthread_t)this->handle, nullptr);
    }

    switch (result) {
    case 0:
    case ESRCH: { // thread already died
        return Result::Success;
    }

    case ETIMEDOUT: {
        return Result::Timeout;
    }

    default: {
        System::Panic("pthread_join/pthread_timedjoin_np failed");
    }
    }
}

bool Thread::IsActive() const {
    if (this->handle == 0) {
        return false;
    }

    const int result = pthread_tryjoin_np((pthread_t)this->handle, nullptr);
    switch (result) {
    case 0: {
        return false;
    }

    case EBUSY: {
        return true;
    }

    default: {
        System::Panic("pthread_tryjoin_np failed");
    }
    }
}

Result Thread::SetName(const String& name) {
    if (this->handle == 0) {
        return Result::Expired;
    }

    if (name.IsEmpty()) {
        return Result::InvalidParameters;
    }

    ScopedBlock nameStr = name.ToCharPointer();
    const int result = pthread_setname_np(this->handle, &nameStr);
    switch (result) {
    case 0: {
        break;
    }

    case ENOENT:
    case ESRCH: {
        return Result::Expired;
    }

    case ERANGE: {
        return Result::InvalidParameters;
    }

    default: {
        System::Panic("pthread_setname_np failed");
    }
    }

    return Result::Success;
}

void Thread::Yield() {
    const int result = sched_yield();
    CELL_ASSERT(result == 0);
}

}
