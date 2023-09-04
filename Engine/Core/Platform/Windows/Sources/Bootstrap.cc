// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Panic.hh>
#include <Cell/System/Platform/Windows.hh>

#include <stdio.h>

#include <timeapi.h>

using namespace Cell;

CELL_UNMANGLED {

__declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001;
__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;

}

int main() {
    // Setup platform fixes

    MMRESULT mmResult = timeBeginPeriod(1);
    CELL_ASSERT(mmResult == MMSYSERR_NOERROR);

    const int stdResult = setvbuf(stdout, nullptr, _IONBF, 0);
    CELL_ASSERT(stdResult == 0);

    // Delegate execution to the platform

    System::Platform::Windows::Start(GetModuleHandleW(nullptr), &CellEntry, !CELL_SYSTEM_IS_TOOL);

    // Cleanup - platform fixes

    mmResult = timeEndPeriod(1);
    CELL_ASSERT(mmResult == MMSYSERR_NOERROR);

    return 0;
}
