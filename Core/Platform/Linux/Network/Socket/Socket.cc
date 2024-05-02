// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Network/Socket.hh>
#include <Cell/System/Panic.hh>

#include <errno.h>
#include <netdb.h>

namespace Cell::Network {

Result Socket::Connect(const AddressInfo* info) {
    addrinfo* infoData = (addrinfo*)info->impl;
    const int result = connect((int)this->impl, infoData->ai_addr, infoData->ai_addrlen);
    if (result == -1) {
        switch (errno) {
        default: {
            System::Panic("connect failed");
        }
        }
    }

    return Result::Success;
}

Result Socket::Disconnect() {
    const int result = shutdown((int)this->impl, SHUT_RDWR);
    if (result == -1) {
        switch (errno) {
        case ENOTCONN: {
            return Result::NotConnected;
        }

        default: {
            System::Panic("shutdown failed");
        }
        }
    }

    return Result::Success;
}

Result Socket::Send(const Memory::IBlock& data, const bool isOutOfBand) {
    if (data.GetSize() > INT32_MAX) {
        return Result::InvalidParameters;
    }

    const ssize_t result = send((int)this->impl, data.AsPointer(), data.GetSize(), isOutOfBand ? MSG_OOB : 0);
    if (result == -1) {
        switch (errno) {
        case ENOMEM: {
            return Result::OutOfMemory;
        }

        default: {
            System::Panic("send failed");
        }
        }
    }
    return Result::Success;
}

Result Socket::Receive(Memory::IBlock& data) {
    if (data.GetSize() > INT32_MAX) {
        return Result::InvalidParameters;
    }

    const ssize_t result = recv((const int)this->impl, data.AsPointer(), (int)data.GetSize(), 0);
    if (result == -1) {
        switch (errno) {
        case ENOMEM: {
            return Result::OutOfMemory;
        }

        default: {
            System::Panic("recv failed");
        }
        }
    }

    return Result::Success;
}

}
