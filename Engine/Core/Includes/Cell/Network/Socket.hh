// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Network/AddressInfo.hh>
#include <Cell/Network/Result.hh>
#include <Cell/System/Block.hh>

namespace Cell::Network {

// Represents a connection between two hosts.
class Socket : public Object {
public:
    // Creates a new socket.
    // By default, it creates a TCP socket via IPv4.
    CELL_FUNCTION static Wrapped<Socket*, Result> New(const Transport transport = Transport::IPv4, const ConnectionType type = ConnectionType::Stream, const Protocol protocol = Protocol::TCP);

    // Closes the socket.
    CELL_FUNCTION ~Socket();

    // Connects with the given address information.
    CELL_FUNCTION Result Connect(const AddressInfo* info);

    // Disconnects a connected socket.
    CELL_FUNCTION Result Disconnect();

    // Sends the given block of memory through the socket.
    CELL_FUNCTION Result Send(const IBlock& data, const bool isOutOfBand = false);

    // Receives the count of data available to the socket and writes it to the given block of memory.
    CELL_FUNCTION Result Receive(IBlock& data);

private:
    CELL_FUNCTION_INTERNAL Socket(const uintptr_t handle) : handle(handle) { }

    uintptr_t handle;
};

}
