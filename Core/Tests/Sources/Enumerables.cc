// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Collection/List.hh>
#include <Cell/System/Entry.hh>

using namespace Cell;
using namespace Cell::Collection;

void CellEntry(Reference<System::String> parameterString) {
    (void)(parameterString);

    List<uint8_t> data((uint8_t)15);
    data.Append(20);
    data.Append(25);
    data.Remove(data.GetCount() - 2);

    CELL_ASSERT(data[0] == 15 && data[1] == 25 && data.GetCount() == 2);
}
