// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/String.hh>
#include <Cell/Network/Properties.hh>
#include <Cell/Network/Result.hh>
#include <Cell/System/Memory.hh>

namespace Cell::Network {

// Represents a collection of addressing information for connection targets.
class AddressInfo : public Object {
friend class Socket;

public:
    // Discovers the necessary addressing information for the given address data.
    CELL_FUNCTION static Wrapped<AddressInfo*, Result> Find(const String&        address,
                                                            const uint16_t       port,
                                                            const Transport      transport = Transport::IPv4,
                                                            const ConnectionType type      = ConnectionType::Stream,
                                                            const Protocol       protocol  = Protocol::TCP
    );

    // Destructs the address information.
    CELL_FUNCTION ~AddressInfo();

    // Returns the number of resolved addresses.
    CELL_FUNCTION size_t GetResolvedCount();

    // Returns either the canonical name or a string representation of the address at the given info index.
    CELL_FUNCTION Wrapped<String, Result> GetName(const size_t infoIndex = 0);

    CELL_NON_COPYABLE(AddressInfo)

private:
    CELL_HANDLE_CONSTRUCTOR(AddressInfo)

    const uintptr_t handle;
};

}
