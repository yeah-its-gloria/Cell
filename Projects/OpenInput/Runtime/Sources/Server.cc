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
    Thread device([](void* p) { ((Server*)p)->DeviceThread(); }, this, "Device Thread");

    Log("Creating the pipe...");
    ScopedObject<IO::Pipe> pipe = IO::Pipe::Create("openinput-1", sizeof(Message)).Unwrap();

    while (this->isActive) {
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

        while (this->isActive) {
            Log("Waiting for message...");

            result = pipe->Read(messageBlock);
            switch (result) {
            case IO::Result::Success: {
                break;
            }

            case IO::Result::Disconnected: {
                Log("Client disconnected.");

                this->isActive = false;
                break;
            }

            default: {
                Panic("Cell::IO::Pipe::Read failed");
            }
            }

            if (!this->isActive) {
                break;
            }

            switch (message.messageType) {
            case MessageType::CreateInstance: {
                const MessageCreateInstance* createInstance = (const MessageCreateInstance*)&message;
                if (createInstance->info.type != OI_STRUCTURE_TYPE_INSTANCE_CREATE_INFO) {
                    Log("Invalid instance create message received, disconnecting");
                    pipe->DisconnectClient();

                    this->isActive = false;
                    break;
                }

                Log(R"(App "%s" (Engine "%s") created a session)", createInstance->info.applicationInfo.applicationName, createInstance->info.applicationInfo.engineName);
                break;
            }

            case MessageType::GetDeviceCount: {
                Log("Requested device count, is %d", this->deviceCount);

                MessageResponseGetDeviceCount response { MessageType::ResponseGetDeviceCount, this->deviceCount, { 0 } };
                pipe->Write(System::UnmanagedBlock { &response });

                break;
            }

            default: {
                Log("Invalid message type received, disconnecting");
                pipe->DisconnectClient();

                this->isActive = false;
                break;
            }
            }
        }
    }

    device.Join(1000);
}

}
