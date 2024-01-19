// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/JavaScript/Engine.hh>
#include <Cell/System/Entry.hh>
#include <Cell/System/Log.hh>

#include <string.h>

using namespace Cell;
using namespace Cell::JavaScript;
using namespace Cell::System;

Value ExampleImpl(Engine* engine) {
    (void)(engine);

    CELL_DEBUGBREAK;
    CELL_UNREACHABLE;
}

void CellEntry(Reference<String> parameterString) {
    (void)(parameterString);

    Log("Testing: engine execution");

    JavaScript::Engine engine;
    Value val = engine.Execute("\"stuff!\"");

    //String a = val.AsString();
    //CELL_ASSERT(a == "stuff!");

    Log("Testing: functions");

    engine.AddFunction(ExampleImpl, "Example");
    Value val2 = engine.Execute("Example(\"stuff\", 2)");
    (void)(val2);
}
