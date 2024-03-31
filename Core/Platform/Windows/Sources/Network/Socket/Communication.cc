// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Network/Socket.hh>
#include <Cell/System/Panic.hh>

#include <Cell/System/Platform/Windows/Includes.h>

#include <WinSock2.h>

namespace Cell::Network {

Result Socket::Send(const Memory::IBlock& data, const bool isOutOfBand) {
    if (data.GetSize() > INT32_MAX) {
        return Result::InvalidParameters;
    }

    WSABUF wsaBuffer = { .len = (uint32_t)data.GetSize(), .buf = (CHAR*)data.AsPointer() };

    DWORD sent = 0;
    const int result = WSASend((SOCKET)this->impl, &wsaBuffer, 1, &sent, isOutOfBand ? MSG_OOB : 0, nullptr, nullptr);
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

Result Socket::Receive(Memory::IBlock& data) {
    if (data.GetSize() > INT32_MAX) {
        return Result::InvalidParameters;
    }

    WSABUF wsaBuffer = { .len = (uint32_t)data.GetSize(), .buf = (char*)data.AsPointer() };

    DWORD received = 0;
    DWORD flags = 0;
    const int result = WSARecv((SOCKET)this->impl, &wsaBuffer, 1, &received, &flags, nullptr, nullptr);
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
