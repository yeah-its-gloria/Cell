// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/DataManagement/RustTest.hh>
#include <Cell/System/Entry.hh>

using namespace Cell;

void CellEntry(Reference<String> parameterString) {
    (void)(parameterString);

    const uint8_t result = DataManagement::RustTest();
    CELL_ASSERT(result == 2);
}
