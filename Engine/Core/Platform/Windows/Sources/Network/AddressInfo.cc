// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
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

}
