// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Example.hh"

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
    delete this->shell;
}

void Example::Launch(const String& parameterString) {
    (void)(parameterString);

    const IO::Result ioResult = IO::CheckPath(this->GetContentPath("/"));
    if (ioResult != IO::Result::Success) {
        Log("Failed to find the content directory, errors might occur");
    }

    this->ShellSetup();

    Thread audio(CELL_THREAD_CLASS_FUNC(Example, AudioThread), "Audio Thread");
    //Thread network(CELL_THREAD_CLASS_FUNC(Example, NetworkThread), "Network Thread");
    Thread renderer(CELL_THREAD_CLASS_FUNC(Example, VulkanThread), "Renderer Thread");

#ifdef CELL_MODULES_OPENXR_AVAILABLE
    Thread xr([](void* p) { ((Example*)p)->XRThread(); }, this, "XR Thread");
#endif

    uint64_t finishedTick = GetPreciseTickerValue();
    while (audio.IsActive()
           // || network.IsActive()
           || renderer.IsActive()
#ifdef CELL_MODULES_OPENXR_AVAILABLE
           || xr.IsActive()
#endif
    ) {
        this->shellDeltaTime = Utilities::Minimum((System::GetPreciseTickerValue() - finishedTick) / 1000.f, 0.001f);

        const Shell::Result result = this->shell->RunDispatch();
        if (result == Shell::Result::RequestedQuit) {
            break;
        }

        CELL_ASSERT(result == Shell::Result::Success);

        finishedTick = GetPreciseTickerValue();
        System::Thread::Yield();
    }

    audio.Join();
    //network.Join();
    renderer.Join();

#ifdef CELL_MODULES_OPENXR_AVAILABLE
    xr.Join();
#endif
}
