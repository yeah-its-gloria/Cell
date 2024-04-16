// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Shell.hh>

#if CELL_PLATFORM_WINDOWS
#include <Cell/Shell/Implementations/Windows.hh>
#elif CELL_PLATFORM_MACOS
#include <Cell/Shell/Implementations/macOS.hh>
#elif CELL_PLATFORM_LINUX
#include <Cell/Shell/Implementations/Linux.hh>
#else
#error No implementation!
#endif

namespace Cell::Shell {

Wrapped<IShell*, Result> CreateShell(const String& title, const Extent extent) {
#ifdef CELL_PLATFORM_WINDOWS
    Wrapped<Implementations::Windows*, Result> result = Implementations::Windows::New(title, extent);
#elif CELL_PLATFORM_MACOS
    Wrapped<Implementations::macOS*, Result> result = Implementations::macOS::New(title, extent);
#elif CELL_PLATFORM_LINUX
    Wrapped<Implementations::Linux*, Result> result = Implementations::Linux::New(title, extent);
#else
#define unimpl
    (void)(title); (void)(extent);

    CELL_UNIMPLEMENTED
#endif

#ifndef unimpl
    if (!result.IsValid()) {
        return result.Result();
    }

    return result.Unwrap();
#else
#undef unimpl
#endif
}

}
