// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#if CELL_PLATFORM_WINDOWS
#include <Cell/Shell/Implementations/Windows.hh>
#elif CELL_PLATFORM_LINUX
#include <Cell/Shell/Implementations/Linux.hh>
#else
#error No implementation!
#endif

namespace Cell::Shell {
using namespace Implementations;

Wrapped<IShell*, Result> CreateShell(const String& title, const Extent extent) {
#ifdef CELL_PLATFORM_WINDOWS
    Wrapped<Windows*, Result> result = Windows::New(title, extent);
#elif CELL_PLATFORM_LINUX
    Wrapped<Linux*, Result> result = Linux::New(title, extent);
#else
    CELL_UNIMPLEMENTED
#endif

    if (!result.IsValid()) {
        return result.Result();
    }

    return result.Unwrap();
}

}
