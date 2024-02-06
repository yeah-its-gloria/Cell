// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Shell.hh>

#if CELL_PLATFORM_WINDOWS
#include <Cell/Shell/Implementations/Windows.hh>
#elif CELL_PLATFORM_LINUX
#include <Cell/Shell/Implementations/Linux.hh>
#else
#error No implementation!
#endif

namespace Cell::Shell {

using namespace Implementations;

Wrapped<IShell*, Result> CreateShell(const System::String& title) {
#ifdef CELL_PLATFORM_WINDOWS
    Wrapped<Windows*, Result> result = Windows::New(title);
#elif CELL_PLATFORM_LINUX
    Wrapped<Linux*, Result> result = Linux::New(title);
#else
    CELL_UNIMPLEMENTED
#endif

    if (!result.IsValid()) {
        return result.Result();
    }

    return result.Unwrap();
}

}
