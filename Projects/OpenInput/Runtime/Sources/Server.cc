// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Server.hh"

#include <Cell/IO/Pipe.hh>
#include <Cell/Scoped.hh>
#include <Cell/System/BlockImpl.hh>
#include <Cell/System/Entry.hh>
#include <Cell/System/Thread.hh>
#include <Runtime.hh>
#include <openinput/openinput.h>

using namespace Cell;
using namespace Cell::System;

void CellEntry(Reference<String> parameterString) {
    (void)(parameterString);

    Runtime::Server().Launch();
}

namespace Runtime {

void Server::Launch() {
    //Thread device([](void* p) { ((Server*)p)->DeviceThread(); }, this, "Device Thread");
    this->DeviceThread();
    return;

    Log("Creating the pipe...");

    ScopedObject<IO::Pipe> pipe = IO::Pipe::Create("openinput-1", sizeof(Message)).Unwrap();

    while (true) {
        Log("Waiting for a client...");
        IO::Result result = pipe->WaitForClient();
        CELL_ASSERT(result == IO::Result::Success);

        const MessageConnect connectMessage = {
            .messageType = MessageType::Connect,
            .runtimeVersion = OI_CURRENT_API_VERSION,
            .padding = {0}};

        Log("Client connected, sending runtime data...");

        UnmanagedBlock connectMsgBlock {&connectMessage};
        result = pipe->Write(connectMsgBlock);
        CELL_ASSERT(result == IO::Result::Success);

        Message message = {
            .messageType = MessageType::Invalid,
            .data = {0}};

        UnmanagedBlock messageBlock {&message};

        bool active = true;
        while (active) {
            Log("Waiting for message...");

            result = pipe->Read(messageBlock);
            switch (result) {
            case IO::Result::Success: {
                break;
            }

            case IO::Result::Disconnected: {
                Log("Client disconnected.");

                active = false;
                break;
            }

            default: {
                Panic("invalid pipe read result");
            }
            }

            if (!active) {
                break;
            }

            switch (message.messageType) {
            case MessageType::CreateInstance: {
                const MessageCreateInstance* createInstance = (const MessageCreateInstance*)&message;
                if (createInstance->info.type != OI_STRUCTURE_TYPE_INSTANCE_CREATE_INFO) {
                    Log("Invalid instance create message received, disconnecting");
                    pipe->DisconnectClient();

                    active = false;
                    break;
                }

                Log(R"(App "%s" (Engine "%s") created a session)", createInstance->info.applicationInfo.applicationName, createInstance->info.applicationInfo.engineName);
                break;
            }

            default: {
                Log("Invalid message type received, disconnecting");
                pipe->DisconnectClient();

                active = false;
                break;
            }
            }
        }
    }

    //device.Join(1000);
}

}
