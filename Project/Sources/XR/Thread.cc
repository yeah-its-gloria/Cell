// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "../Example.hh"

#include <Cell/Scoped.hh>
#include <Cell/OpenXR/Session.hh>
#include <Cell/System/Log.hh>

using namespace Cell;

void Example::XRThread() {
    Wrapped<OpenXR::Instance*, OpenXR::Result> instanceResult = OpenXR::Instance::New();
    switch (instanceResult.Result()) {
    case OpenXR::Result::Success: {
        break;
    }

    case OpenXR::Result::RuntimeUnavailable: {
        System::Log("Failed to initialize OpenXR; runtime isn't available.");
        return;
    }

    case OpenXR::Result::RuntimeFailure: {
        System::Log("Failed to initialize OpenXR; runtime isn't functioning.");
        return;
    }

    default: {
        System::Panic("Cell::OpenXR::Instance::New failed");
    }
    }

    ScopedObject instance = instanceResult.Unwrap();

    D3D12::Device* device = D3D12::Device::New().Unwrap();

    Wrapped<OpenXR::Session*, OpenXR::Result> sessionResult = instance->CreateSessionD3D12(OpenXR::FormFactor::HeadMountedDisplay, device);
    switch (sessionResult.Result()) {
    case OpenXR::Result::Success: {
        break;
    }

    case OpenXR::Result::Unavailable: {
        System::Log("Failed to initialize OpenXR system; no headset connected.");
        return;
    }

    default: {
        System::Panic("Cell::OpenXR::Instance::CreateSession failed");
    }
    }

    ScopedObject session = sessionResult.Unwrap();

    while (true) {
        const Shell::Result result = shell->RunDispatch();
        if (result == Shell::Result::RequestedQuit) {
            break;
        }

        CELL_ASSERT(result == Shell::Result::Success);
        System::Thread::Yield();
    }
}
