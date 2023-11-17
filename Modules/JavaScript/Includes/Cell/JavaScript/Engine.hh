// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/System/String.hh>

namespace Cell::JavaScript {

typedef class Engine Engine;
typedef class Value Value;

typedef Value (Function)(Engine* engine);

class Engine : public Object {
public:
    CELL_FUNCTION Engine();
    CELL_FUNCTION ~Engine();

    CELL_FUNCTION void AddFunction(Function& function, const System::String& name);
    CELL_FUNCTION Value Execute(const System::String& content);

private:
    void* context;
    void* runtime;
};

class Value : public Object {
friend Engine;

public:
    CELL_FUNCTION ~Value();

    CELL_FUNCTION System::String AsString();

private:
    CELL_INLINE Value(Engine* ref, void* val) : reference(ref), value(val) { }

    Engine* reference;
    void* value;
};

}
