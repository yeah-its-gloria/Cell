// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/IO/Pipe.hh>
#include <Cell/System/Panic.hh>
#include <Cell/System/Platform/Windows/Includes.h>

namespace Cell::IO {

Pipe::~Pipe() {
    CloseHandle((HANDLE)this->handle);
}

Result Pipe::Read(Memory::IBlock& data) {
    if (data.GetSize() > UINT32_MAX) {
        return Result::InvalidParameters;
    }

    DWORD read = 0;
    const BOOL fileResult = ReadFile((HANDLE)this->handle, data.AsPointer(), (DWORD)data.GetSize(), &read, nullptr);
    if (fileResult == FALSE) {
        switch (GetLastError()) {
        case ERROR_BROKEN_PIPE: {
            if (!this->isClient) {
                Result result = this->DisconnectClient();
                CELL_ASSERT(result == Result::Success);
            }

            return Result::Disconnected;
        }

        case ERROR_MORE_DATA: {
            return Result::Incomplete;
        }

        default: {
            System::Panic("ReadFile failed");
        }
        }
    }

    if (read != data.GetSize()) {
        return Result::Incomplete;
    }

    return Result::Success;
}

Result Pipe::Write(const Memory::IBlock& data) {
    if (data.GetSize() > UINT32_MAX) {
        return Result::InvalidParameters;
    }

    DWORD written = 0;
    const BOOL fileResult = WriteFile((HANDLE)this->handle, data.AsPointer(), (DWORD)data.GetSize(), &written, nullptr);
    if (fileResult == FALSE) {
        switch (GetLastError()) {
        case ERROR_BROKEN_PIPE: {
            if (!this->isClient) {
                Result result = this->DisconnectClient();
                CELL_ASSERT(result == Result::Success);
            }

            return Result::Disconnected;
        }

        default: {
            System::Panic("ReadFile failed");
        }
        }
    }

    if (written != data.GetSize()) {
        return Result::Incomplete;
    }

    return Result::Success;
}

}
