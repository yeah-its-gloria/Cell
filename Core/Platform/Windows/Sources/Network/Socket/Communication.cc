// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "../Internal.hh"

#include <Cell/Network/Socket.hh>
#include <Cell/System/Panic.hh>

namespace Cell::Network {

Result Socket::Send(const IBlock& data, const bool isOutOfBand) {
    if (data.ByteSize() > INT32_MAX) {
        return Result::InvalidParameters;
    }

    WSABUF wsaBuffer = { .len = (uint32_t)data.ByteSize(), .buf = (char*)data.Pointer() };

    DWORD sent = 0;
    const int result = WSASend((SOCKET)this->handle, &wsaBuffer, 1, &sent, isOutOfBand ? MSG_OOB : 0, nullptr, nullptr);
    if (result == SOCKET_ERROR) {
        switch (WSAGetLastError()) {
        case WSAECONNABORTED:
        case WSAECONNRESET:
        case WSAENETRESET:
        case WSAENETDOWN: {
            return Result::LostConnection;
        }

        case WSAEMSGSIZE: {
            return Result::ContentTooLarge;
        }

        case WSAENOTCONN:
        case WSAESHUTDOWN: {
            return Result::NotConnected;
        }

        default: {
            System::Panic("WSASend failed");
        }
        }
    }

    return Result::Success;
}

Result Socket::Receive(IBlock& data) {
    if (data.ByteSize() > INT32_MAX) {
        return Result::InvalidParameters;
    }

    WSABUF wsaBuffer = { .len = (uint32_t)data.ByteSize(), .buf = (char*)data.Pointer() };

    DWORD received = 0;
    DWORD flags = 0;
    const int result = WSARecv((SOCKET)this->handle, &wsaBuffer, 1, &received, &flags, nullptr, nullptr);
    if (result == SOCKET_ERROR) {
        switch (WSAGetLastError()) {
        case WSAECONNABORTED:
        case WSAECONNRESET:
        case WSAENETRESET:
        case WSAENETDOWN: {
            return Result::LostConnection;
        }

        case WSAEMSGSIZE: {
            return Result::ContentTooLarge;
        }

        case WSAENOTCONN:
        case WSAESHUTDOWN: {
            return Result::NotConnected;
        }

        default: {
            System::Panic("WSARecv failed");
        }
        }
    }

    return Result::Success;
}

}
