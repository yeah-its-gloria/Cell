// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Network/Socket.hh>
#include <Cell/System/Panic.hh>

#include <errno.h>
#include <netdb.h>

namespace Cell::Network {

Result Socket::Connect(const AddressInfo* info) {
    addrinfo* infoData = (addrinfo*)info->handle;
    const int result = connect((int)this->handle, infoData->ai_addr, infoData->ai_addrlen);
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
    const int result = shutdown((int)this->handle, SHUT_RDWR);
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

Result Socket::Send(const IBlock& data, const bool isOutOfBand) {
    if (data.ByteSize() > INT32_MAX) {
        return Result::InvalidParameters;
    }

    const ssize_t result = send((int)this->handle, data.Pointer(), data.ByteSize(), isOutOfBand ? MSG_OOB : 0);
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

Result Socket::Receive(IBlock& data) {
    if (data.ByteSize() > INT32_MAX) {
        return Result::InvalidParameters;
    }

    const ssize_t result = recv((const int)this->handle, data.Pointer(), (int)data.ByteSize(), 0);
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
