// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/System/Event.hh>
#include <Cell/System/Panic.hh>
#include <Cell/System/Thread.hh>

#include <errno.h>
#include <pthread.h>

namespace Cell::System {

struct threadData {
    Event* event;
    ThreadFunction function;
    void* parameter;
};

void* thread_trampoline(void* param) {
    struct threadData data = *(struct threadData*)param;

    data.event->Signal();

    data.function(data.parameter);
    return nullptr;
}

Thread::Thread(ThreadFunction function, void* parameter, const String& name) {
    Event event;

    const threadData data = {
        .event     = &event,
        .function  = function,
        .parameter = parameter
    };

    pthread_t thread = 0;
    int result = pthread_create(&thread, nullptr, thread_trampoline, (void**)&data);
    CELL_ASSERT(result == 0);

    event.Wait();

    this->handle = (uintptr_t)thread;

    if (!name.IsEmpty()) {
        this->SetName(name);
    }
}

Thread::~Thread() {
    pthread_cancel((pthread_t)this->handle);
}

Result Thread::Join(const uint32_t timeout_ms) const {
    int result = 0;

    if (timeout_ms > 0) {
        struct timespec timeout = { .tv_sec = 0, .tv_nsec = timeout_ms * 1000 };
        if (timeout_ms >= 1000) {
            timeout.tv_sec = timeout_ms / 1000;
            timeout.tv_nsec = (timeout_ms % 1000) * 1000;
        }

        result = pthread_timedjoin_np((pthread_t)this->handle, nullptr, &timeout);
    } else {
        result = pthread_join((pthread_t)this->handle, nullptr);
    }

    switch (result) {
    case 0:
    case ESRCH: // thread already died
    {
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
    int result = pthread_tryjoin_np((pthread_t)this->handle, nullptr);
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

Result Thread::SetName(const System::String& name) {
    if (name.IsEmpty()) {
        return Result::InvalidParameters;
    }

    ScopedBlock<char> nameStr = name.ToCharPointer();
    const int result = pthread_setname_np(this->handle, &nameStr);
    switch (result) {
    case 0: {
        break;
    }

    case ENOENT:
    case ESRCH: {
        return Result::ElementHasDied;
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
