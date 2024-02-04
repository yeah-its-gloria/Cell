// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Entry.hh>
#include <Cell/System/Event.hh>

using namespace Cell;
using namespace Cell::System;

void CellEntry(Reference<String> parameterString) {
    (void)(parameterString);

    Event event;
    CELL_ASSERT(event.Wait(1) == EventWaitResult::Timeout);

    event.Signal();
    CELL_ASSERT(event.Wait(1) == EventWaitResult::Signaled);

    event.Signal();
    CELL_ASSERT(event.Wait(1) == EventWaitResult::Signaled);

    event.Reset();
    CELL_ASSERT(event.Wait(1) == EventWaitResult::Timeout);

    event.Reset();
    CELL_ASSERT(event.Wait(1) == EventWaitResult::Timeout);
}
