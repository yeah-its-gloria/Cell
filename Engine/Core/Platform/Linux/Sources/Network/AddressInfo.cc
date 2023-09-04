// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Internal.hh"

#include <Cell/Scoped.hh>
#include <Cell/Network/AddressInfo.hh>
#include <Cell/System/Panic.hh>

#include <netdb.h>
#include <stdio.h>

namespace Cell::Network {

Wrapped<AddressInfo*, Result> AddressInfo::Get(const System::String& address, const uint16_t port, const Transport transport, const ConnectionType type, const Protocol protocol) {
    if (address.IsEmpty() || port == 0) {
        return Result::InvalidParameters;
    }

    Wrapped<systemTypes, Result> result = convertPropertiesToSystemTypes(transport, type, protocol);
    if (!result.IsValid()) {
        return result.Result();
    }

    systemTypes types = result.Unwrap();

    const addrinfo hints = {
        .ai_flags     = 0,
        .ai_family    = types.transport,
        .ai_socktype  = types.type,
        .ai_protocol  = types.protocol,
        .ai_addrlen   = 0,
        .ai_addr      = nullptr,
        .ai_canonname = nullptr,
        .ai_next      = nullptr
    };

    char portStr[6] = { 0, 0, 0, 0, 0, 0 };
    snprintf(portStr, 6, "%d", port);

    addrinfo* results = nullptr;

    ScopedBlock<char> addressStr = address.ToCharPointer();
    const int sockResult = getaddrinfo(&addressStr, portStr, &hints, &results);
    switch (sockResult) {
    case 0: {
        break;
    }

    case EAI_AGAIN: {
        return Result::ResolutionFailure;
    }

    case EAI_NONAME: {
        return Result::HostNotFound;
    }

    case EAI_NODATA: {
        return Result::PortNotFound;
    }

    case EAI_MEMORY: {
        return Result::OutOfMemory;
    }

    default: {
        System::Panic("getaddrinfo failed");
    }
    }

    AddressInfo* stuff = new AddressInfo((uintptr_t)results);
    return stuff;
}

AddressInfo::~AddressInfo() {
    freeaddrinfo((addrinfo*)this->handle);
}

}
