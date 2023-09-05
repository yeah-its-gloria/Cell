// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Example.hh"

#include <Cell/Network/AddressInfo.hh>
#include <Cell/Network/Socket.hh>
#include <Cell/Scoped.hh>
#include <Cell/System/BlockImpl.hh>
#include <Cell/Utilities.hh>

using namespace Cell;
using namespace Cell::System;

const char* prebuiltRequest = "GET / HTTP/1.1\r\nHost: example.com\r\nUser-Agent: Cell/1.0.0\r\nAccept: */*\r\n\r\n";

void Example::NetworkThread() {
    ScopedObject<Network::Socket> socket = Network::Socket::New().Unwrap();
    ScopedObject<Network::AddressInfo> info = Network::AddressInfo::Get("example.com", 80).Unwrap();

    Network::Result result = socket->Connect(&info);
    CELL_ASSERT(result == Network::Result::Success);

    result = socket->Send(UnmanagedBlock {prebuiltRequest, Utilities::RawStringSize(prebuiltRequest)});
    CELL_ASSERT(result == Network::Result::Success);

    ManagedBlock<uint8_t> received(2048);
    result = socket->Receive(received);
    CELL_ASSERT(result == Network::Result::Success);

    result = socket->Disconnect();
    CELL_ASSERT(result == Network::Result::Success);
}
