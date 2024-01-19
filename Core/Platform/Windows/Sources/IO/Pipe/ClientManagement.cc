// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/IO/Pipe.hh>
#include <Cell/System/Panic.hh>
#include <Cell/System/Platform/Windows/Includes.h>

namespace Cell::IO {

Result Pipe::WaitForClient() {
    if (this->isClient) {
        return Result::InvalidOperation;
    }

    const BOOL result = ConnectNamedPipe((HANDLE)this->handle, nullptr);
    if (result == FALSE) {
        switch (GetLastError()) {
        case ERROR_NO_DATA: {
            // client just disconnected, this error can be ignored safely
            break;
        }

        default: {
            System::Panic("ConnectNamedPipe failed");
        }
        }
    }

    return Result::Success;
}

Result Pipe::DisconnectClient() {
    if (this->isClient) {
        return Result::InvalidOperation;
    }

    const BOOL result = DisconnectNamedPipe((HANDLE)this->handle);
    if (result == FALSE) {
        System::Panic("DisconnectNamedPipe failed");
    }

    return Result::Success;
}

}
