// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "../Internal.hh"

#include <Cell/Network/Socket.hh>
#include <Cell/System/Panic.hh>

namespace Cell::Network {

Wrapped<Socket*, Result> Socket::New(const Transport transport, const ConnectionType type, const Protocol protocol) {
    Wrapped<systemTypes, Result> result = convertPropertiesToSystemTypes(transport, type, protocol);
    if (result.Result() != Result::Success) {
        return result.Result();
    }

    systemTypes types = result.Unwrap();
    const SOCKET _socket = WSASocketW(types.transport, types.type, types.protocol, nullptr, 0, WSA_FLAG_OVERLAPPED);
    if (_socket == INVALID_SOCKET) {
        switch (WSAGetLastError()) {
        case WSANOTINITIALISED: {
            return Result::PlatformNotReadyYet;
        }

        case WSAENETDOWN: {
            return Result::NetworkUnavailable;
        }

        default: {
            Cell::System::Panic("WSASocketW failed");
        }
        }
    }

    return new Socket((uintptr_t)_socket);
}

Socket::~Socket() {
    shutdown(this->handle, SD_BOTH); // best effort attempt : )

    const int result = closesocket(this->handle);
    CELL_ASSERT(result == 0);
}

}
