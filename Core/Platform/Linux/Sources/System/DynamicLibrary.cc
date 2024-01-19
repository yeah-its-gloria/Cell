// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/System/DynamicLibrary.hh>
#include <Cell/System/Panic.hh>

#include <dlfcn.h>
#include <errno.h>

namespace Cell::System {

Wrapped<DynamicLibrary*, Result> DynamicLibrary::New(const String& path) {
    if (path.IsEmpty()) {
        return Result::InvalidParameters;
    }

    ScopedBlock<char> pathStr = path.ToCharPointer();
    void* handle = dlopen(&pathStr, RTLD_LAZY);
    if (handle == nullptr) {
        const int a = errno;
        (void)(a);
        System::Panic("dlopen failed");
    }

    return new DynamicLibrary((uintptr_t)handle);
}

DynamicLibrary::~DynamicLibrary() {
    dlclose((void*)this->handle);
}

}
