// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Entry.hh>
#include <Cell/System/Log.hh>

using namespace Cell;

void CellEntry(Reference<String> parameterString) {
    (void)(parameterString);

    String a = "Hello";
    String b = " World";

    String c = a + b;

    CELL_ASSERT(a == "Hello");
    CELL_ASSERT(b == " World");
    CELL_ASSERT(c == "Hello World");

    //String euro = "€";

    //CELL_ASSERT(euro.GetSize() == 3);
    //CELL_ASSERT(euro.GetCount() == 1);

    String d = "hi";
    uint32_t e = 30;

    System::Log(String::Format("% %", d, e));
}
