// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
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

size_t AddressInfo::GetResolvedCount() {
    size_t i = 1;

    addrinfo* ptr = (addrinfo*)this->handle;
    for (; i < SIZE_MAX; i++) {
        ptr = ptr->ai_next;
        if (ptr == nullptr) {
            break;
        }
    }

    return i;
}

Wrapped<System::String, Result> AddressInfo::GetName(const size_t infoIndex) {
    addrinfo* ptr = (addrinfo*)this->handle;
    for (size_t i = 1; i < infoIndex; i++) {
        ptr = ptr->ai_next;
        if (ptr == nullptr) {
            return Result::OutOfRange;
        }
    }

    if (ptr->ai_flags & AI_CANONNAME) {
        return System::String(ptr->ai_canonname);
    }

    switch (ptr->ai_family) {
    case AF_INET: {
        sockaddr_in* addr = (sockaddr_in*)ptr->ai_addr;
        return System::String::Format("%d.%d.%d.%d",
                                      (addr->sin_addr.s_addr >> 24) & 0xff,
                                      (addr->sin_addr.s_addr >> 16) & 0xff,
                                      (addr->sin_addr.s_addr >>  8) & 0xff,
                                       addr->sin_addr.s_addr        & 0xff);
    }

    case AF_INET6: {
        sockaddr_in6* addr = (sockaddr_in6*)ptr->ai_addr;
        return System::String::Format("%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x",
                                      ((uint16_t)addr->sin6_addr.s6_addr[1]  << 8) | addr->sin6_addr.s6_addr[0],
                                      ((uint16_t)addr->sin6_addr.s6_addr[3]  << 8) | addr->sin6_addr.s6_addr[2],
                                      ((uint16_t)addr->sin6_addr.s6_addr[5]  << 8) | addr->sin6_addr.s6_addr[4],
                                      ((uint16_t)addr->sin6_addr.s6_addr[7]  << 8) | addr->sin6_addr.s6_addr[6],
                                      ((uint16_t)addr->sin6_addr.s6_addr[9]  << 8) | addr->sin6_addr.s6_addr[8],
                                      ((uint16_t)addr->sin6_addr.s6_addr[11] << 8) | addr->sin6_addr.s6_addr[10],
                                      ((uint16_t)addr->sin6_addr.s6_addr[13] << 8) | addr->sin6_addr.s6_addr[12],
                                      ((uint16_t)addr->sin6_addr.s6_addr[15] << 8) | addr->sin6_addr.s6_addr[14]);
    }

    default: {
        break;
    }
    }

    CELL_UNREACHABLE;
}

}
