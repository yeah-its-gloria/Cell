// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/BlockImpl.hh>
#include <Cell/System/Entry.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Panic.hh>
#include <Cell/System/Platform/Windows/Includes.h>

#include <stdio.h>

#include <roapi.h>
#include <WinSock2.h>

using namespace Cell;

// NOLINTBEGIN (readability-identifier-naming)

CELL_UNMANGLED {

__declspec(dllexport) uint32_t AmdPowerXpressRequestHighPerformance = 0x00000001;
__declspec(dllexport) uint32_t NvOptimusEnablement = 0x00000001;

int32_t NtQueryTimerResolution(uint32_t* minimum, uint32_t* maximum, uint32_t* current);
int32_t NtSetTimerResolution(uint32_t desired, BOOL set, uint32_t* current);

}

// NOLINTEND

int main() {
    uint32_t _ = 0, desired = 0;
    int32_t ntResult = NtQueryTimerResolution(&_, &desired, &_);
    CELL_ASSERT(ntResult == ERROR_SUCCESS);

    if (desired > 5000) {
        System::Log("Timer resolution %d ns is above 50 ns!", desired / 100);
    }

    ntResult = NtSetTimerResolution(desired, TRUE, &_);
    CELL_ASSERT(ntResult == ERROR_SUCCESS);

    const int stdResult = setvbuf(stdout, nullptr, _IONBF, 0);
    CELL_ASSERT(stdResult == 0);

    WSADATA wsaData;
    int wsaResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    CELL_ASSERT(wsaResult == 0);

    const HRESULT comResult = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    CELL_ASSERT(SUCCEEDED(comResult));

    const wchar_t* rawCmdLine = GetCommandLineW();
    const size_t rawCmdLineSize = wcslen(rawCmdLine);

    size_t firstSpaceIndex = 0;
    for (size_t i = 0; i < rawCmdLineSize; i++) {
        if (rawCmdLine[i] == L' ') {
            firstSpaceIndex = i;
            break;
        }
    }

    System::String parameters = "";
    if (firstSpaceIndex + 1 != rawCmdLineSize) {
        const size_t desiredCmdSize = rawCmdLineSize - firstSpaceIndex;

        System::OwnedBlock<wchar_t> stuff(desiredCmdSize);
        System::CopyMemory<wchar_t>(stuff, rawCmdLine + (firstSpaceIndex + 1), desiredCmdSize - 2);

        parameters += System::String::FromPlatformWideString(stuff).Unwrap();
    }

    CellEntry(Reference(parameters));

    CoUninitialize();

    wsaResult = WSACleanup();
    CELL_ASSERT(wsaResult == 0);

    return 0;
}
