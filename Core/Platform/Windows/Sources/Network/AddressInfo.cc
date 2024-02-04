// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Internal.hh"

#include <Cell/Scoped.hh>
#include <Cell/Network/AddressInfo.hh>
#include <Cell/System/Panic.hh>

#include <stdio.h>

// why Microsoft
#define CopyMemory RtlCopyMemory
#include <ws2tcpip.h>
#undef CopyMemory

namespace Cell::Network {

Wrapped<AddressInfo*, Result> AddressInfo::Get(const System::String& address, const uint16_t port, const Transport transport, const ConnectionType type, const Protocol protocol) {
    if (address.IsEmpty() || port == 0) {
        return Result::InvalidParameters;
    }

    Wrapped<systemTypes, Result> result = convertPropertiesToSystemTypes(transport, type, protocol);
    if (!result.IsValid()) {
        return result.Result();
    }

    const systemTypes types = result.Unwrap();

    const ADDRINFOW hints = {
        .ai_flags     = 0,
        .ai_family    = types.transport,
        .ai_socktype  = types.type,
        .ai_protocol  = types.protocol,
        .ai_addrlen   = 0,
        .ai_canonname = nullptr,
        .ai_addr      = nullptr,
        .ai_next      = nullptr
    };

    wchar_t portWide[6] = { 0, 0, 0, 0, 0, 0 };
    swprintf_s(portWide, 6, L"%d", port);

    ADDRINFOW* results = nullptr;

    ScopedBlock<wchar_t> addressWide = address.ToPlatformWideString();
    const INT wsaResult = GetAddrInfoW(&addressWide, portWide, &hints, &results);
    switch (wsaResult) {
    case 0: {
        break;
    }

    case WSATRY_AGAIN: {
        return Result::ResolutionFailure;
    }

    case WSAHOST_NOT_FOUND: {
        return Result::HostNotFound;
    }

    case WSATYPE_NOT_FOUND: {
        return Result::PortNotFound;
    }

    default: {
        System::Panic("GetAddrInfoW failed");
    }
    }

    AddressInfo* stuff = new AddressInfo((uintptr_t)results);
    return stuff;
}

AddressInfo::~AddressInfo() {
    FreeAddrInfoW((ADDRINFOW*)this->handle);
}

size_t AddressInfo::GetResolvedCount() {
    size_t i = 1;

    ADDRINFOW* ptr = (ADDRINFOW*)this->handle;
    for (; i < SIZE_MAX; i++) {
        ptr = ptr->ai_next;
        if (ptr == nullptr) {
            break;
        }
    }

    return i;
}

Wrapped<System::String, Result> AddressInfo::GetName(const size_t infoIndex) {
    ADDRINFOW* ptr = (ADDRINFOW*)this->handle;
    for (size_t i = 1; i < infoIndex; i++) {
        ptr = ptr->ai_next;
        if (ptr == nullptr) {
            return Result::OutOfRange;
        }
    }

    if (ptr->ai_flags & AI_CANONNAME) {
        return System::String::FromPlatformWideString(ptr->ai_canonname).Unwrap();
    }

    switch (ptr->ai_family) {
    case AF_INET: {
        sockaddr_in* addr = (sockaddr_in*)ptr->ai_addr;
        return System::String::Format("%.%d.%d.%d",
                                      addr->sin_addr.S_un.S_un_b.s_b1, addr->sin_addr.S_un.S_un_b.s_b2,
                                      addr->sin_addr.S_un.S_un_b.s_b3, addr->sin_addr.S_un.S_un_b.s_b4);
    }

    case AF_INET6: {
        sockaddr_in6* addr = (sockaddr_in6*)ptr->ai_addr;
        return System::String::Format("%:%x:%:%:%:%:%:%",
                                      addr->sin6_addr.u.Word[0], addr->sin6_addr.u.Word[1],
                                      addr->sin6_addr.u.Word[2], addr->sin6_addr.u.Word[3],
                                      addr->sin6_addr.u.Word[4], addr->sin6_addr.u.Word[5],
                                      addr->sin6_addr.u.Word[6], addr->sin6_addr.u.Word[7]);
    }

    default: {
        break;
    }
    }

    CELL_UNREACHABLE;
}

}
