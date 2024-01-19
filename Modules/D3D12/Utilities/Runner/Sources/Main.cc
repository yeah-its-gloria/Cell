// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/D3D12/Device.hh>
#include <Cell/System/Entry.hh>
#include <Cell/System/Thread.hh>

using namespace Cell;

void CellEntry(Reference<System::String> parameterString) {
    (void)(parameterString);

    ScopedObject<Shell::IShell> shell = Shell::CreateShell("Cell - D3D12 Runner").Unwrap();

    ScopedObject<D3D12::Device> device = D3D12::Device::New().Unwrap();

    D3D12::Result result = device->CreateSwapchain(shell);
    CELL_ASSERT(result == D3D12::Result::Success);

    while (true) {
        const Shell::Result result = shell->RunDispatch();
        if (result == Shell::Result::RequestedQuit) {
            break;
        }

        CELL_ASSERT(result == Shell::Result::Success || result == Shell::Result::NoUpdates);
        System::Thread::Yield();
    }

    return;
}
