// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/System/Event.hh>
#include <Cell/System/Panic.hh>
#include <Cell/System/Platform/Windows/Includes.h>
#include <Cell/System/Thread.hh>

namespace Cell::System {

struct threadData {
    Event* event;
    ThreadFunction function;
    void* parameter;
};

DWORD threadTrampoline(void* param) {
    threadData data = *(threadData*)param;
    data.event->Signal();

    data.function(data.parameter);
    return 0;
}

Thread::Thread(ThreadFunction function, void* parameter, const String& name) {
    Event event;

    threadData data = {
        .event = &event,
        .function = function,
        .parameter = parameter
    };

    HANDLE thread = CreateThread(nullptr, 0, threadTrampoline, &data, 0, nullptr);
    CELL_ASSERT(thread != nullptr);

    event.Wait();

    this->handle = (uintptr_t)thread;
    if (!name.IsEmpty()) {
        Result result = this->SetName(name);
        CELL_ASSERT(result == Result::Success);
    }
}

Thread::~Thread() {
    const BOOL result = TerminateThread((HANDLE)this->handle, 0);
    CELL_ASSERT(result);
}

Result Thread::Join(const uint32_t timeMilliseconds) const {
    const DWORD result = WaitForSingleObjectEx((HANDLE)this->handle, timeMilliseconds == 0 ? INFINITE : timeMilliseconds, FALSE);
    switch (result) {
    case WAIT_OBJECT_0: {
        return Result::Success;
    }

    case WAIT_TIMEOUT: {
        return Result::Timeout;
    }

    default: {
        System::Panic("WaitForSingleObjectEx for Thread::Join failed");
    }
    }
}

bool Thread::IsActive() const {
    const DWORD result = WaitForSingleObjectEx((HANDLE)this->handle, 0, FALSE);
    switch (result) {
    case WAIT_OBJECT_0: {
        return false;
    }

    case WAIT_TIMEOUT: {
        return true;
    }

    default: {
        System::Panic("WaitForSingleObjectEx for Thread::IsActive failed");
    }
    }
}

Result Thread::SetName(const String& name) {
    if (name.IsEmpty()) {
        return Result::InvalidParameters;
    }

    if (!this->IsActive()) {
        return Result::ElementHasDied;
    }

    ScopedBlock<wchar_t> nameWide = name.ToPlatformWideString();
    const HRESULT result = SetThreadDescription((HANDLE)this->handle, &nameWide);
    CELL_ASSERT(SUCCEEDED(result) || result == 0x10000000);

    return Result::Success;
}

void Thread::Yield() {
    SwitchToThread();
}

}
