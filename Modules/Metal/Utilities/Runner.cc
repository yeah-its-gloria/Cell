// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/Metal/Device.hh>
#include <Cell/System/Entry.hh>
#include <Cell/System/Timer.hh>

using namespace Cell;

void CellEntry(Reference<String> parameterString) {
    (void)(parameterString);

    ScopedObject<Shell::IShell> shell = Shell::CreateShell("Cell - Metal").Unwrap();

    ScopedObject<Metal::Device> device = Metal::Device::New(shell).Unwrap();

    while (true) {
        const Shell::Result result = shell->RunDispatch();
        if (result == Shell::Result::RequestedQuit) {
            break;
        }

        CELL_ASSERT(result == Shell::Result::Success);
        System::Sleep(1);
    }
}
