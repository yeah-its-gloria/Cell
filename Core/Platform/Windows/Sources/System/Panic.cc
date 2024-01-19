// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Panic.hh>
#include <Cell/System/Platform/Windows/Includes.h>

namespace Cell::System {

void SoftPanic() {
    MessageBoxW(nullptr, L"An error occurred that caused the engine and/or title to crash.", L"Cell", MB_OK | MB_ICONERROR);

    ExitProcess(0);
}

}
