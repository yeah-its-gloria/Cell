// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Entry.hh>
#include <Cell/System/Event.hh>

using namespace Cell;
using namespace Cell::System;

void CellEntry(Reference<String> parameterString) {
    (void)(parameterString);

    Event event;
    CELL_ASSERT(event.Wait(1) == EventWaitState::Timeout);

    event.Signal();
    CELL_ASSERT(event.Wait(1) == EventWaitState::Signaled);

    event.Signal();
    CELL_ASSERT(event.Wait(1) == EventWaitState::Signaled);

    event.Reset();
    CELL_ASSERT(event.Wait(1) == EventWaitState::Timeout);

    event.Reset();
    CELL_ASSERT(event.Wait(1) == EventWaitState::Timeout);

    String a = "Hello";
    String b = " World";

    String c = a + b;

    CELL_ASSERT(a == "Hello");
    CELL_ASSERT(b == " World");
    CELL_ASSERT(c == "Hello World");
}
