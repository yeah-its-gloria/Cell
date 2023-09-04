// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/System/DynamicLibrary.hh>
#include <Cell/System/Panic.hh>
#include <Cell/System/Platform/Windows/Includes.h>

namespace Cell::System {

Wrapped<DynamicLibrary*, Result> DynamicLibrary::New(const String& path) {
    if (path.IsEmpty()) {
        return Result::InvalidParameters;
    }

    ScopedBlock<wchar_t> widePath = path.ToPlatformWideString();
    const HMODULE module = LoadLibraryExW(&widePath, nullptr, 0);
    if (module == nullptr) {
        switch (GetLastError()) {
            // TODO: figure out what results this returns

        default: {
            Panic("LoadLibraryExW failed");
        }
        }
    }

    return new DynamicLibrary((uintptr_t)module);
}

DynamicLibrary::~DynamicLibrary() {
    const BOOL result = FreeLibrary((HMODULE)this->handle);
    CELL_ASSERT(result == TRUE);
}

Wrapped<GenericFunctionPointer, Result> DynamicLibrary::GetFunction(const String& name) {
    if (this->loadedFunctions.Has(name).IsValid()) {
        return this->loadedFunctions.GetValue(name);
    }

    ScopedBlock<char> namePtr = name.ToCharPointer();
    FARPROC proc = GetProcAddress((HMODULE)this->handle, &namePtr);
    if (proc == nullptr) {
        switch (GetLastError()) {
        case ERROR_PROC_NOT_FOUND: {
            return Result::NotFound;
        }

        default: {
            Panic("GetProcAddress failed");
        }
        }
    }

    this->loadedFunctions.Append(name, (GenericFunctionPointer)proc);
    return (GenericFunctionPointer)proc;
}

}
