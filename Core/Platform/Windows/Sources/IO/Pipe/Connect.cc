// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/IO/Pipe.hh>
#include <Cell/System/Platform/Windows/Includes.h>

#define HAS_MODE(in) (((uint8_t)(PipeMode::in) & (uint8_t)mode) == (uint8_t)(PipeMode::in))

namespace Cell::IO {

Wrapped<Pipe*, Result> Pipe::Connect(const String& name, const PipeMode mode) {
    if (name.IsEmpty() || (!HAS_MODE(Read) && !HAS_MODE(Write))) {
        return Result::InvalidParameters;
    }

    DWORD pipeMode = 0;

    if (HAS_MODE(Read)) {
        pipeMode |= GENERIC_READ;
    }

    if (HAS_MODE(Write)) {
        pipeMode |= GENERIC_WRITE;
    }

    String pipeName = String("\\\\.\\pipe\\") + name;
    ScopedBlock pipeNameWide = pipeName.ToPlatformWideString();

    const HANDLE handle = CreateFileW(
        pipeNameWide,
        pipeMode,
        0,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr
    );

    if (handle == INVALID_HANDLE_VALUE) {
        switch (GetLastError()) {
        default: {
            System::Panic("CreateFileW failed");
        }
        }
    }

    return new Pipe((uintptr_t)handle, true);
}

}
