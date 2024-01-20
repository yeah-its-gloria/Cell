// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Example.hh"

#include <Cell/IO/File.hh>
#include <Cell/System/Entry.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Thread.hh>
#include <Cell/System/Timer.hh>

using namespace Cell;
using namespace Cell::System;

void CellEntry(Reference<String> parameterString) {
    Example().Launch(parameterString.Unwrap());
}

CELL_FUNCTION_INTERNAL Example::~Example() {
    delete this->shell;
}

void Example::Launch(const String& parameterString) {
    (void)(parameterString);

    const IO::Result ioResult = IO::CheckPath(this->GetContentPath("/"));
    if (ioResult != IO::Result::Success) {
        Log("Failed to find the content directory, errors might occur");
    }

    this->shell = Shell::CreateShell("Cell - Hi Aurelia").Unwrap();

    Thread audio([](void* p) { ((Example*)p)->AudioThread(); }, this, "Audio Thread");
    Thread input([](void* p) { ((Example*)p)->InputThread(); }, this, "Input Thread");
    //Thread network([](void* p) { ((Example*)p)->NetworkThread(); }, this, "Network Thread");
    Thread renderer([](void* p) { ((Example*)p)->VulkanThread(); }, this, "Renderer Thread");

#ifdef CELL_MODULES_OPENXR_AVAILABLE
    Thread xr([](void* p) { ((Example*)p)->XRThread(); }, this, "XR Thread");
#endif

    const uint64_t startTick = GetPreciseTickerValue();
    while (audio.IsActive()
           || input.IsActive()
           // || network.IsActive()
           || renderer.IsActive()
#ifdef CELL_MODULES_OPENXR_AVAILABLE
           || xr.IsActive()
#endif
    ) {
        const Shell::Result result = this->shell->RunDispatch();
        if (result == Shell::Result::RequestedQuit) {
            break;
        }

        CELL_ASSERT(result == Shell::Result::Success || result == Shell::Result::NoUpdates);
        Thread::Yield();

        this->elapsedTime = (GetPreciseTickerValue() - startTick) / 1000.f;
    }

    audio.Join(500);
    input.Join(500);
    //network.Join();
    renderer.Join(500);

#ifdef CELL_MODULES_OPENXR_AVAILABLE
    xr.Join();
#endif
}
