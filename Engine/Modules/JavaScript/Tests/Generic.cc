// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/JavaScript/Engine.hh>
#include <Cell/System/Entry.hh>
#include <Cell/System/Log.hh>

#include <string.h>

using namespace Cell;

void CellEntry(Reference<System::String> parameterString) {
    (void)(parameterString);

    // TODO: better testing

    System::Log("Testing: magnitude");

    JavaScript::Engine engine;
    JSValue val = engine.RunScript("\"stuff!\"");

    ScopedBlock<char> logData = engine.ValueToString(val).ToCharPointer();
    CELL_ASSERT(strcmp(&logData, "stuff!") == 0);
}
