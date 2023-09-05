// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Example.hh"

#include <Cell/IO/File.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Thread.hh>

using namespace Cell;
using namespace Cell::System;

void CellEntry(Reference<String> parameterString) {
    Example().Launch(parameterString.Unwrap());
}

CELL_FUNCTION_INTERNAL Example::~Example() {
    delete this->input;
    delete this->shell;
}

void Example::Launch(const String& parameterString) {
    (void)(parameterString);

    const IO::Result ioResult = IO::File::CheckPath("./Projects/Example/Content");
    if (ioResult != IO::Result::Success) {
        Log("Failed to find the content directory, errors might occur");
    }

    this->shell = Shell::CreateShell("Cell - Hi Aurelia").Unwrap();
    this->input = this->shell->CreateInputHandler();

    Thread audio([](void* p) { ((Example*)p)->AudioThread(); }, this, "Audio Thread");
    Thread network([](void* p) { ((Example*)p)->NetworkThread(); }, this, "Network Thread");
    Thread renderer([](void* p) { ((Example*)p)->VulkanThread(); }, this, "Renderer Thread");
    Thread xr([](void* p) { ((Example*)p)->XRThread(); }, this, "XR Thread");

    while (audio.IsActive() || network.IsActive() || renderer.IsActive() || xr.IsActive()) {
        const Shell::Result result = this->shell->RunDispatch();
        if (result == Shell::Result::RequestedQuit) {
            break;
        }

        CELL_ASSERT(result == Shell::Result::Success);
        Thread::Yield();
    }

    audio.Join();
    network.Join();
    renderer.Join();
    xr.Join();
}
