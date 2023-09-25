// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/System/String.hh>

#include <quickjs.h>

namespace Cell::JavaScript {

class Engine : public Object {
public:
    CELL_FUNCTION Engine();
    CELL_FUNCTION ~Engine();

    CELL_FUNCTION JSValue RunScript(const System::String& content);
    CELL_FUNCTION System::String ValueToString(JSValue value);

private:
    JSContext* context;
    JSRuntime* runtime;
};

}
