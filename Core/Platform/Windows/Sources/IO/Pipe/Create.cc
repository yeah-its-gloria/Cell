// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/IO/Pipe.hh>
#include <Cell/System/Platform/Windows/Includes.h>

#define HAS_MODE(in) (((uint8_t)(PipeMode::in) & (uint8_t)mode) == (uint8_t)(PipeMode::in))

namespace Cell::IO {

Wrapped<Pipe*, Result> Pipe::Create(const String& name, const size_t blockSize, const PipeMode mode) {
    if (name.IsEmpty() || blockSize == 0 || blockSize > UINT32_MAX || (!HAS_MODE(Read) && !HAS_MODE(Write))) {
        return Result::InvalidParameters;
    }

    DWORD pipeMode = 0;

    if (HAS_MODE(Read)) {
        pipeMode |= PIPE_ACCESS_INBOUND;
    }

    if (HAS_MODE(Write)) {
        pipeMode |= PIPE_ACCESS_OUTBOUND;
    }

    String pipeName = String("\\\\.\\pipe\\") + name;
    ScopedBlock pipeNameWide = pipeName.ToPlatformWideString();

    const HANDLE handle = CreateNamedPipeW(
        pipeNameWide,
        pipeMode | FILE_FLAG_FIRST_PIPE_INSTANCE,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT | PIPE_REJECT_REMOTE_CLIENTS,
        PIPE_UNLIMITED_INSTANCES,
        (DWORD)blockSize,
        (DWORD)blockSize,
        NMPWAIT_USE_DEFAULT_WAIT,
        nullptr
    );

    if (handle == INVALID_HANDLE_VALUE) {
        switch (GetLastError()) {
        case ERROR_ACCESS_DENIED: {
            return Result::AlreadyExists;
        }

        default: {
            System::Panic("CreateNamedPipeW failed");
        }
        }
    }

    return new Pipe((uintptr_t)handle);
}

}
