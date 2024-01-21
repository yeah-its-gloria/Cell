// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Entry.hh>
#include <Cell/System/Panic.hh>

#include <Cell/Utilities/Byteswap.hh>
#include <Cell/Utilities/MinMaxClamp.hh>
#include <Cell/Utilities/RawString.hh>

using namespace Cell::Utilities;

void CellEntry(Cell::Reference<Cell::System::String> parameterString) {
    (void)(parameterString);

    CELL_ASSERT(Byteswap((uint16_t)0xf0ff) == 0xfff0);
    CELL_ASSERT(Byteswap(0xf0fffeff) == 0xfffefff0);
    CELL_ASSERT(Byteswap(0xf0fffeff00fffefe) == 0xfefeff00fffefff0);

    CELL_ASSERT(Clamp(2, 1, 3) == 2);
    CELL_ASSERT(Clamp(0, 1, 3) == 1);
    CELL_ASSERT(Clamp(4, 1, 3) == 3);

    CELL_ASSERT(RawStringSize("ABCDEF") == 6);
}
