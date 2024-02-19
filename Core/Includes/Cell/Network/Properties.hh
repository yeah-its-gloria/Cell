// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::Network {

// Types of transports.
enum class Transport : uint8_t {
    // Unspecified transport.
    Unspecified,

    // Transport via IP version 4.
    IPv4,

    // Transport via IP version 6.
    IPv6
};

// Various methods of connection and communication.
enum class ConnectionType : uint8_t {
    // Represents a stream connection.
    Stream,

    // Represents a datagram connection.
    Datagram,

    // Represents a bare connection.
    Raw
};

// Various implemented protocols.
enum class Protocol : uint8_t {
    // Transmission Control Protocol; slow but reliable.
    TCP,

    // User Datagram Protocol; lossy but quick.
    UDP,

    // Internet Control Message Protocol; useful for pings and such.
    ICMP
};

}
