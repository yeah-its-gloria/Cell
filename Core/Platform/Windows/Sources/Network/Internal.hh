// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Wrapped.hh>
#include <Cell/Network/Properties.hh>
#include <Cell/Network/Result.hh>
#include <Cell/System/Platform/Windows/Includes.h>

#include <WinSock2.h>

namespace Cell::Network {

struct systemTypes {
    int transport;
    int type;
    int protocol;
};

CELL_INLINE Wrapped<systemTypes, Result> convertPropertiesToSystemTypes(const Transport transport, const ConnectionType type, const Protocol protocol) {
    systemTypes types { };

    switch (transport) {
    case Transport::Unspecified: {
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

    case Transport::Bluetooth: {
        types.transport = AF_BTH;
        break;
    }

    default: {
        return Result::InvalidParameters;
    }
    }

    switch (type) {
    case ConnectionType::Stream: {
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
