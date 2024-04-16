// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/System/Event.hh>
#include <Cell/System/Panic.hh>
#include <Cell/System/Thread.hh>

#include <Foundation/NSString.h>
#include <Foundation/NSThread.h>

#include <pthread/sched.h>

@interface CellThreadImpl : NSObject
-(id) initWithFunction: (Cell::System::ThreadFunction) function andParameter: (void*) parameter;
-(void) trampoline: (id) unused;

@property Cell::System::ThreadFunction function;
@property void* parameter;
@end

@implementation CellThreadImpl
-(id) initWithFunction: (Cell::System::ThreadFunction) func andParameter: (void*) param {
   self = [super init];

   self.function = func;
   self.parameter = param;

   return self;
}

-(void) trampoline: (id) unused {
    self.function(self.parameter);
}
@end


namespace Cell::System {

struct threadData {
    CellThreadImpl* object;
    NSThread* thread;
};

Thread::Thread(ThreadFunction function, void* parameter, const String& name) {
    CellThreadImpl* object = [[CellThreadImpl alloc] initWithFunction: function andParameter: parameter];
    CELL_ASSERT(object != nullptr);

    NSThread* thread = [[NSThread alloc] initWithTarget:object selector:@selector(trampoline:) object:nullptr];
    CELL_ASSERT(thread != nullptr);

    threadData* data = Memory::Allocate<threadData>();

    data->object = object;
    data->thread = thread;

    this->impl = (uintptr_t)data;

    if (!name.IsEmpty()) {
        //this->SetName(name);
    }

    [thread start];
}


Thread::~Thread() {
    threadData* data = (threadData*)this->impl;

    //[data->thread dealloc];
    //[data->object dealloc];

    Memory::Free(data);
}

Result Thread::Join(const uint32_t timeoutMs) const {
    // TODO: timeouts

    (void)(timeoutMs);

    threadData* data = (threadData*)this->impl;

    while ([data->thread isExecuting] == YES) {
        Thread::Yield();
    }

    return Result::Success;
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

    NSString* nsName = [[NSString alloc] initWithBytes: name.ToRawPointer() length: name.GetSize() encoding: NSUTF8StringEncoding];
    [data->thread setName: nsName];

    [nsName dealloc];
    return Result::Success;
}

void Thread::Yield() {
    sched_yield();
}

}
