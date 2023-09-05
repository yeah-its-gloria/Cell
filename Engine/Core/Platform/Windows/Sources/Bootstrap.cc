// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Entry.hh>
#include <Cell/System/Panic.hh>
#include <Cell/System/Platform/Windows/Includes.h>

#include <stdio.h>

#include <roapi.h>
#include <timeapi.h>
#include <WinSock2.h>

using namespace Cell;

// NOLINTBEGIN (readability-identifier-naming)

CELL_UNMANGLED {

__declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001;
__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;

}

// NOLINTEND

int main() {
    MMRESULT mmResult = timeBeginPeriod(1);
    CELL_ASSERT(mmResult == MMSYSERR_NOERROR);

    const int stdResult = setvbuf(stdout, nullptr, _IONBF, 0);
    CELL_ASSERT(stdResult == 0);

    WSADATA wsaData;
    int wsaResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    CELL_ASSERT(wsaResult == 0);

    const HRESULT comResult = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    CELL_ASSERT(SUCCEEDED(comResult));

    System::String a = "";
    CellEntry(Reference(a));

    CoUninitialize();

    wsaResult = WSACleanup();
    CELL_ASSERT(wsaResult == 0);

    mmResult = timeEndPeriod(1);
    CELL_ASSERT(mmResult == MMSYSERR_NOERROR);

    return 0;
}
