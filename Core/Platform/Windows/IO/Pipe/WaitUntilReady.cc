// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/IO/Pipe.hh>
#include <Cell/System/Platform/Windows/Includes.h>

namespace Cell::IO {

Result Pipe::WaitUntilReady(const String& name, const uint32_t timeoutMilliseconds) {
    if (name.IsEmpty()) {
        return Result::InvalidParameters;
    }

    String pipeName = String("\\\\.\\pipe\\") + name;
    ScopedBlock pipeNameWide = pipeName.ToPlatformWideString();

    const BOOL result = WaitNamedPipeW(pipeNameWide, timeoutMilliseconds == 0 ? NMPWAIT_WAIT_FOREVER : timeoutMilliseconds);
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
