// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/JavaScript/Engine.hh>

#include <stdlib.h>

namespace Cell::JavaScript {

Engine::Engine() {
    this->runtime = JS_NewRuntime();
    CELL_ASSERT(this->runtime != nullptr);

    this->context = JS_NewContext(this->runtime);
    CELL_ASSERT(this->context != nullptr);

    JS_AddIntrinsicBigFloat(this->context);
    JS_AddIntrinsicBigDecimal(this->context);
    JS_AddIntrinsicOperators(this->context);
    JS_EnableBignumExt(this->context, true);
}

Engine::~Engine() {
    JS_FreeContext(this->context);
    JS_FreeRuntime(this->runtime);
}

JSValue Engine::RunScript(const System::String& content) {
    ScopedBlock<char> contentStr = content.ToCharPointer();

    return JS_Eval(this->context, contentStr, content.GetLength(), "EngineScript.js", 0);
}


System::String Engine::ValueToString(JSValue value) {
    char* data = JS_CopyStringToNewBuffer(JS_VALUE_GET_STRING(value));
    System::String string = System::String(data);
    free(data);
    return string;
}

}
