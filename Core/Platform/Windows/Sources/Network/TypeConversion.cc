// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Internal.hh"

namespace Cell::Network {

Wrapped<systemTypes, Result> convertPropertiesToSystemTypes(const Transport transport, const ConnectionType type, const Protocol protocol) {
    systemTypes types { };

    switch (transport) {
    case Transport::Unspecified: {
        types.transport = AF_UNSPEC;
        break;
    }

    case Transport::IPv4: {
        types.transport = AF_INET;
        break;
    }

    case Transport::IPv6: {
        types.transport = AF_INET6;
        break;
    }

    default: {
        return Result::InvalidParameters;
    }
    }

    switch (type) {
    case ConnectionType::Stream: {
        types.type = SOCK_STREAM;
        break;
    }

    case ConnectionType::Datagram: {
        types.type = SOCK_DGRAM;
        break;
    }

    case ConnectionType::Raw: {
        types.type = SOCK_RAW;
        break;
    }

    default: {
        return Result::InvalidParameters;
    }
    }

    switch (protocol) {
    case Protocol::TCP: {
        types.protocol = IPPROTO_TCP;
        break;
    }

    case Protocol::UDP: {
        types.protocol = IPPROTO_UDP;
        break;
    }

    case Protocol::ICMP: {
        switch (transport) {
        case Transport::IPv4: {
            types.protocol = IPPROTO_ICMP;
            break;
        }

        case Transport::IPv6: {
            types.protocol = IPPROTO_ICMPV6;
            break;
        }

        default: {
            return Result::InvalidParameters;
        }
        }

        break;
    }

    default: {
        return Result::InvalidParameters;
    }
    }

    return types;
}

}
