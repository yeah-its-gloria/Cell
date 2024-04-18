// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/System/DynamicLibrary.hh>

#include <dlfcn.h>
#include <errno.h>

namespace Cell::System {

Wrapped<DynamicLibrary*, Result> DynamicLibrary::New(const String& path) {
    if (path.IsEmpty()) {
        return Result::InvalidParameters;
    }

    ScopedBlock<char> pathStr = path.ToCharPointer();
    void* handle = dlopen(&pathStr, RTLD_NOW);
    if (handle == nullptr) {
        switch (errno) {
        case ENOENT: {
            return Result::NotFound;
        }

        case EACCES: {
            return Result::AccessDenied;
        }

        case ENOMEM: {
            return Result::OutOfMemory;
        }

        default: {
            System::Panic("dlopen failed");
        }
        }
    }

    return new DynamicLibrary((uintptr_t)handle);
}

DynamicLibrary::~DynamicLibrary() {
    dlclose((void*)this->impl);
}

Wrapped<GenericFunctionPointer, Result> DynamicLibrary::GetFunction(const String& name) {
    if (this->loadedFunctions.Has(name)) {
        return this->loadedFunctions.GetValue(name);
    }

    ScopedBlock<char> nameStr = name.ToCharPointer();

    GenericFunctionPointer function = (GenericFunctionPointer)dlsym((void*)this->impl, nameStr);
    if (function == nullptr) {
        switch (errno) {
        case ENOENT: {
            return Result::NotFound;
        }

        default: {
            System::Panic("dlsym failed");
        }
        }
    }

    this->loadedFunctions.Append(name, function);
    return function;
}

}
