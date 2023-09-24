// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Dictionary.hh>

#include <Cell/System/Result.hh>
#include <Cell/System/String.hh>

namespace Cell::System {

// Generic function pointer.
typedef void (* GenericFunctionPointer)();

// Manages a dynamically loaded library.
class DynamicLibrary : public Object {
public:
    // Loads a library.
    CELL_FUNCTION static Wrapped<DynamicLibrary*, Result> New(const String& path);

    // Unloads the library.
    CELL_FUNCTION ~DynamicLibrary();

    // Loads the function with the given name, and returns a pointer for it.
    CELL_FUNCTION Wrapped<GenericFunctionPointer, Result> GetFunction(const String& name);

private:
    CELL_FUNCTION_INTERNAL DynamicLibrary(const uintptr_t handle) : handle(handle) { }

    uintptr_t handle;
    Dictionary<String, GenericFunctionPointer> loadedFunctions;
};

}
