// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Memory/Block.hh>
#include <Cell/Network/AddressInfo.hh>

namespace Cell::Network {

// Represents a connection between two hosts.
class Socket : public NoCopyObject {
public:
    // Creates a new socket.
    // By default, it creates a TCP socket via IPv4.
    CELL_FUNCTION static Wrapped<Socket*, Result> New(const Transport      transport = Transport::IPv4,
                                                      const ConnectionType type      = ConnectionType::Stream,
                                                      const Protocol       protocol  = Protocol::TCP);

    // Closes the socket.
    CELL_FUNCTION ~Socket();

    // Connects with the given address information.
    CELL_FUNCTION Result Connect(const AddressInfo* info);

    // Disconnects a connected socket.
    CELL_FUNCTION Result Disconnect();

    // Sends the given block through the socket.
    CELL_FUNCTION Result Send(const Memory::IBlock& data, const bool isOutOfBand = false);

    // Receives the count of data available to the socket and writes it to the given block.
    CELL_FUNCTION Result Receive(Memory::IBlock& data);

private:
    CELL_FUNCTION_INTERNAL Socket(uintptr_t i) : impl(i) { }

    uintptr_t impl;
};

}
