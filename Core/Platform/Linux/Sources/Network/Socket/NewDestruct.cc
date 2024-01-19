// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "../Internal.hh"

#include <Cell/Network/Socket.hh>

#include <unistd.h>

namespace Cell::Network {

Wrapped<Socket*, Result> Socket::New(const Transport transport, const ConnectionType type, const Protocol protocol) {
    Wrapped<systemTypes, Result> result = convertPropertiesToSystemTypes(transport, type, protocol);
    if (!result.IsValid()) {
        return result.Result();
    }

    const systemTypes types = result.Unwrap();

    const int _socket = ::socket(types.transport, types.type, types.protocol);
    if (_socket == -1) {
        Cell::System::Panic("socket failed");
    }

    return new Socket((uintptr_t)_socket);
}

Socket::~Socket() {
    const int result = close((int)this->handle);
    CELL_ASSERT(result == 0);
}

}
