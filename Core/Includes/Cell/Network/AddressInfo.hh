// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Wrapped.hh>

#include <Cell/Network/Properties.hh>
#include <Cell/Network/Result.hh>

#include <Cell/System/Memory.hh>
#include <Cell/System/String.hh>

namespace Cell::Network {

class Socket;

// Represents a collection of information on a given address.
class AddressInfo : public Object {
friend Socket;

public:
    // Fetches information for the given address data and collects it as an address information object.
    static CELL_FUNCTION Wrapped<AddressInfo*, Result> Get(const System::String& address,
                                                           const uint16_t port,
                                                           const Transport transport = Transport::IPv4,
                                                           const ConnectionType type = ConnectionType::Stream,
                                                           const Protocol protocol = Protocol::TCP
    );

    // Destructs the address information.
    CELL_FUNCTION ~AddressInfo();

    // Returns the number of resolved addresses.
    CELL_FUNCTION size_t GetResolvedCount();

    // Returns either the canonical name or a string representation of the address at the given info index.
    CELL_FUNCTION Wrapped<System::String, Result> GetName(const size_t infoIndex = 0);

private:
    CELL_FUNCTION_INTERNAL AddressInfo(const uintptr_t handle) : handle(handle) { };

    uintptr_t handle;
};

}
