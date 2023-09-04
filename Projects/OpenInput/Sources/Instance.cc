// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/IO/Pipe.hh>
#include <Cell/System/BlockImpl.hh>
#include <Cell/System/Memory.hh>
#include <Cell/System/Panic.hh>
#include <Runtime.hh>
#include <openinput/openinput.h>

using namespace Cell;
using namespace Runtime;

struct OiInstance_T {
    IO::Pipe* pipe;
};

OiResult oiCreateInstance(const OiInstanceCreateInfo createInfo, OiInstance* instance) {
    (void)(createInfo);

    CELL_ASSERT(instance != nullptr);

    IO::Result result = IO::Pipe::WaitUntilReady("openinput-1");
    CELL_ASSERT(result == IO::Result::Success);

    OiInstance_T* inst = System::AllocateMemory<struct OiInstance_T>();
    CELL_ASSERT(inst != nullptr);

    inst->pipe = IO::Pipe::Connect("openinput-1").Unwrap();

    MessageConnect connectMessage = {
        .messageType = MessageType::Connect,
        .runtimeVersion = 0,
        .padding = {0}};

    System::UnmanagedBlock connectMsgBlock {&connectMessage};
    result = inst->pipe->Read(connectMsgBlock);
    CELL_ASSERT(result == IO::Result::Success);

    CELL_ASSERT(connectMessage.messageType == MessageType::Connect && connectMessage.runtimeVersion == OI_CURRENT_API_VERSION);

    MessageCreateInstance createInstanceMessage = {
        .messageType = MessageType::CreateInstance,
        .info = createInfo};

    System::UnmanagedBlock createInstMsgBlock {&createInstanceMessage};
    result = inst->pipe->Write(createInstMsgBlock);
    CELL_ASSERT(result == IO::Result::Success);

    *instance = inst;
    return OI_SUCCESS;
}

OiResult oiDestroyInstance(OiInstance instance) {
    CELL_ASSERT(instance != nullptr);

    struct OiInstance_T* inst = instance;

    delete inst->pipe;
    System::FreeMemory(inst);

    return OI_SUCCESS;
}

OiResult oiRequestUsers(OiInstance instance, const uint8_t count) {
    (void)(instance);
    (void)(count);

    return OI_SUCCESS;
}

OiResult oiAcquireUpdates(OiInstance instance) {
    (void)(instance);

    return OI_SUCCESS;
}
