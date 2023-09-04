// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Panic.hh>
#include <Cell/System/Platform/Linux.hh>

#include <stdio.h>

using namespace Cell;

int main() {
    // Setup platform fixes

    const int std_result = setvbuf(stdout, nullptr, _IONBF, 0);
    CELL_ASSERT(std_result == 0);

    // Delegate execution to the platform

    System::Platform::Linux::Start(&CellEntry, !CELL_SYSTEM_IS_TOOL);

    return 0;
}
