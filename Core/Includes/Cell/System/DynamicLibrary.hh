// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/String.hh>
#include <Cell/Collection/Dictionary.hh>
#include <Cell/System/Result.hh>

namespace Cell::System {

// Generic function pointer.
typedef void (* GenericFunctionPointer)();

// Manages a dynamically loaded library.
class DynamicLibrary : public NoCopyObject {
public:
    // Loads a library.
    CELL_FUNCTION static Wrapped<DynamicLibrary*, Result> New(const String& path);

    // Unloads the library.
    CELL_FUNCTION ~DynamicLibrary();

    // Loads the function with the given name, and returns a pointer for it.
    CELL_FUNCTION Wrapped<GenericFunctionPointer, Result> GetFunction(const String& name);

private:
    CELL_FUNCTION_INTERNAL DynamicLibrary(uintptr_t i) : impl(i) { }

    uintptr_t impl;
    Collection::Dictionary<String, GenericFunctionPointer> loadedFunctions;
};

}
