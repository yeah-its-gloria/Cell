// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "../Internal.hh"

#include <Cell/Network/Socket.hh>
#include <Cell/System/Panic.hh>

namespace Cell::Network {

Result Socket::Connect(const AddressInfo* info) {
    ADDRINFOW* infoData = (ADDRINFOW*)info->impl;
    const int result = WSAConnect(this->impl, infoData->ai_addr, (int)infoData->ai_addrlen, nullptr, nullptr, nullptr, nullptr);
    if (result == SOCKET_ERROR) {
        switch (WSAGetLastError()) {
        default: {
            System::Panic("WSAConnect failed");
        }
        }
    }

    return Result::Success;
}

Result Socket::Disconnect() {
    const int result = shutdown(this->impl, SD_BOTH);
    if (result == SOCKET_ERROR) {
        switch (WSAGetLastError()) {
        case WSAENOTCONN: {
            return Result::NotConnected;
        }

        default: {
            System::Panic("shutdown failed");
        }
        }
    }

    return Result::Success;
}

}
