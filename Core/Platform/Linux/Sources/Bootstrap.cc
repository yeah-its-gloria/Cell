// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Entry.hh>

#include <stdio.h>

using namespace Cell;

int main() {
    const int std_result = setvbuf(stdout, nullptr, _IONBF, 0);
    CELL_ASSERT(std_result == 0);

    String a = "";
    CellEntry(Reference(a));

    return 0;
}
