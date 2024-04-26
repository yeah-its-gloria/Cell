// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Memory/Allocator.hh>
#include <Cell/System/Thread.hh>

#include <Foundation/Foundation.h>

namespace Cell::System {

struct threadData {
    NSThread* thread;
    NSCondition* finished;
};

Thread::Thread(ThreadFunction function, void* parameter, const String& name) {
    NSCondition* finished = [NSCondition new];

    NSThread* thread = [[NSThread alloc] initWithBlock: ^{
        function(parameter);

        [finished lock];
        [finished broadcast];
    }];

    threadData* data = Memory::Allocate<threadData>();

    data->thread = thread;
    data->finished = finished;

    this->impl = (uintptr_t)data;

    if (!name.IsEmpty()) {
        this->SetName(name);
    }

    [thread start];
}

Thread::~Thread() {
    Memory::Free((threadData*)this->impl);
}

Result Thread::Join(const uint32_t timeoutMs) const {
    threadData* data = (threadData*)this->impl;

    if ([data->thread isFinished] == YES) {
        return Result::Success;
    }

    [data->finished lock];

    if (timeoutMs == 0) {
        [data->finished wait];
        [data->finished unlock];
        return Result::Success;
    }

    const BOOL result = [data->finished waitUntilDate: [NSDate dateWithTimeIntervalSinceNow: ((double)timeoutMs / MSEC_PER_SEC)]];
    [data->finished unlock];

    if (result == YES) {
        return Result::Success;
    }

    return Result::Timeout;
}

bool Thread::IsActive() const {
    threadData* data = (threadData*)this->impl;
    return [data->thread isFinished] == NO;
}

Result Thread::SetName(const String& name) {
    if (name.IsEmpty()) {
        return Result::InvalidParameters;
    }

    threadData* data = (threadData*)this->impl;

    NSString* nsName = name.ToPlatformNSString();
    NSString* lockName = [nsName stringByAppendingString: @" Join Lock"];

    [data->thread setName: nsName];
    [data->finished setName: lockName];

    return Result::Success;
}

void Thread::Yield() {
    [NSThread sleepForTimeInterval: 1.0 / MSEC_PER_SEC];
}

}
