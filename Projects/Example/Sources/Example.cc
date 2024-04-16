// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "InputController.hh"

#include <Cell/IO/File.hh>
#include <Cell/System/Entry.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Timer.hh>
#include <Cell/Utilities/MinMaxClamp.hh>

using namespace Cell;
using namespace Cell::System;

void CellEntry(Reference<String> parameterString) {
    Example().Launch(parameterString.Unwrap());
}

CELL_FUNCTION_INTERNAL Example::~Example() {
    if (this->shell != nullptr) {
        delete this->shell;
    }
}

void Example::Launch(const String& parameterString) {
    (void)(parameterString);

    const IO::Result ioResult = IO::CheckPath(this->GetContentPath("/"));
    switch (ioResult) {
    case IO::Result::Success: {
        break;
    }

    case IO::Result::NotFound: {
        Log("Failed to find the content directory.");
        return;
    }

    case IO::Result::AccessDenied:
    case IO::Result::Locked: {
        Log("The content directory is inaccessible.");
        return;
    }

    default: {
        Log("The content directory cannot be accessed due to an indeterminate failure.");
        return;
    }
    }

    this->shell = Shell::CreateShell("Cell - Hi Aurelia").Unwrap();

    Shell::Result result = this->shell->DiscoverPeripherals();
    if (result != Shell::Result::Success) {
        System::Panic("Failed to discover input peripherals");
    }

    this->controller = InputController::New(this);

    //Thread audio(CELL_THREAD_CLASS_FUNC(Example, AudioThread), "Audio Thread");
    Thread renderer(CELL_THREAD_CLASS_FUNC(Example, RendererThread), "Renderer Thread");

#ifdef CELL_MODULES_OPENXR_AVAILABLE
    Thread xr([](void* p) { ((Example*)p)->XRThread(); }, this, "XR Thread");
#endif

    uint64_t finishedTick = GetPreciseTickerValue();
    while (//audio.IsActive()
           /*||*/ renderer.IsActive()
#ifdef CELL_MODULES_OPENXR_AVAILABLE
           || xr.IsActive()
#endif
    ) {
        this->shellDeltaTime = Utilities::Minimum((GetPreciseTickerValue() - finishedTick) / 1000.f, 0.001f);

        result = this->shell->RunDispatch();
        if (result == Shell::Result::RequestedQuit) {
            break;
        }

        CELL_ASSERT(result == Shell::Result::Success);

        finishedTick = GetPreciseTickerValue();
        Thread::Yield();
    }

    //audio.Join();
    renderer.Join();

#ifdef CELL_MODULES_OPENXR_AVAILABLE
    xr.Join();
#endif
}
