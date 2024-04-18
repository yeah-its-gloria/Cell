// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Entry.hh>
#include <Cell/System/Event.hh>

#include <Cell/Scoped.hh>
#include <Cell/IO/File.hh>
#include <Cell/Memory/OwnedBlock.hh>

using namespace Cell;
using namespace Cell::System;

void CellEntry(Reference<String> parameterString) {
    (void)(parameterString);

    Event event;
    CELL_ASSERT(event.Wait(1) == Result::Timeout);

    event.Signal();
    CELL_ASSERT(event.Wait(1) == Result::Success);

    event.Signal();
    CELL_ASSERT(event.Wait(1) == Result::Success);

    event.Reset();
    CELL_ASSERT(event.Wait(1) == Result::Timeout);

    event.Reset();
    CELL_ASSERT(event.Wait(1) == Result::Timeout);
}
