// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/Network/Socket.hh>
#include <Cell/System/BlockImpl.hh>
#include <Cell/System/Entry.hh>

using namespace Cell;
using namespace Cell::Network;
using namespace Cell::System;

void CellEntry(Reference<String> parameterString) {
    (void)(parameterString);

    const char* request = "GET / HTTP/1.1\r\nHost: example.com\r\nUser-Agent: Cell/1.0.0\r\nAccept: */*\r\n\r\n";

    ScopedObject info = Network::AddressInfo::Find("example.com", 80).Unwrap();
    ScopedObject socket = Network::Socket::New().Unwrap();

    Network::Result result = socket->Connect(&info);
    CELL_ASSERT(result == Network::Result::Success);

    result = socket->Send(UnownedBlock { request, StringDetails::RawStringSize(request) });
    CELL_ASSERT(result == Network::Result::Success);

    OwnedBlock<uint8_t> received(2048);
    result = socket->Receive(received);
    CELL_ASSERT(result == Network::Result::Success);

    // I hope the folks at example.com enjoy my spam lol

    result = socket->Disconnect();
    CELL_ASSERT(result == Network::Result::Success);
}
