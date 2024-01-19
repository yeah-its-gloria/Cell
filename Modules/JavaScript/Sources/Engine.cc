// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/JavaScript/Engine.hh>

#include <quickjs.h>

#include <stdlib.h>

namespace Cell::JavaScript {

Engine::Engine() {
    this->runtime = (void*)JS_NewRuntime();
    CELL_ASSERT(this->runtime != nullptr);

    this->context = (void*)JS_NewContext((JSRuntime*)this->runtime);
    CELL_ASSERT(this->context != nullptr);

    JS_AddIntrinsicBigFloat((JSContext*)this->context);
    JS_AddIntrinsicBigDecimal((JSContext*)this->context);
    JS_AddIntrinsicOperators((JSContext*)this->context);
    JS_EnableBignumExt((JSContext*)this->context, true);
}

Engine::~Engine() {
    JS_FreeContext((JSContext*)this->context);
    JS_FreeRuntime((JSRuntime*)this->runtime);
}

void Engine::AddFunction(Function& function, const System::String& name) {
    auto func = [](JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) -> JSValue {
        (void)(ctx); (void)(this_val); (void)(argc); (void)(argv);

        CELL_UNREACHABLE;
    };

    (void)(function);

    ScopedBlock<char> nameStr = name.ToCharPointer();
    JS_NewCFunction((JSContext*)this->context, (JSCFunction*)func, nameStr, 0);
}

Value Engine::Execute(const System::String& content) {
    ScopedBlock<char> contentStr = content.ToCharPointer();

    JSValue value = JS_Eval((JSContext*)this->context, contentStr, content.GetSize(), "EngineScript.js", 0);
    (void)(value);
    return Value(this, nullptr);
}


Value::~Value() {
    (void)(this->value);
    (void)(this->reference);
}

System::String Value::AsString() {
    //char* data = nullptr; //JS_CopyStringToNewBuffer(JS_VALUE_GET_STRING((JSValue)value));

    //System::String string = System::String(data);

    //free(data);
    return System::String("");
}

}
