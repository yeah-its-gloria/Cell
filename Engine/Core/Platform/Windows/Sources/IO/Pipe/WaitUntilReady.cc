// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/IO/Pipe.hh>
#include <Cell/System/Panic.hh>
#include <Cell/System/Platform/Windows/Includes.h>

namespace Cell::IO {

Result Pipe::WaitUntilReady(const System::String& name, const uint32_t timeoutMilliseconds) {
    if (name.IsEmpty()) {
        return Result::InvalidParameters;
    }

    System::String pipeName = "\\\\.\\pipe\\";
    pipeName += name;

    const BOOL result = WaitNamedPipeW(pipeName.ToPlatformWideString(), timeoutMilliseconds == 0 ? NMPWAIT_WAIT_FOREVER : timeoutMilliseconds);
    if (result == FALSE) {
        switch (GetLastError()) {
        case ERROR_NOT_FOUND: {
            return Result::NotFound;
        }

        case ERROR_SEM_TIMEOUT: {
            return Result::Timeout;
        }

        default: {
            System::Panic("WaitNamedPipeW failed");
        }
        }
    }

    return Result::Success;
}

}
