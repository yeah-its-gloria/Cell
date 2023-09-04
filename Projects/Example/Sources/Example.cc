// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Example.hh"

#include <Cell/IO/File.hh>
#include <Cell/Shell/Window.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Thread.hh>

using namespace Cell;
using namespace Cell::System;

void CellEntry(Reference<IPlatform> platform, Reference<String> parameterString) {
    Example(platform.Unwrap()).Launch(parameterString.Unwrap());
}

void Example::Launch(const String& parameterString) {
    (void)(parameterString);

    const IO::Result ioResult = IO::File::CheckPath("./Projects/Example/Content");
    if (ioResult != IO::Result::Success) {
        Log("Failed to find the content directory, errors might occur");
    }

    this->platform.BlockUntilReady();

    this->input = this->platform.CreateInputHandler();

    Shell::SetNewTitleForWindow(this->platform, "Cell - Hi Aurelia");

    Thread audio([](void* p) { ((Example*)p)->AudioThread(); }, this, "Audio Thread");
    Thread network([](void* p) { ((Example*)p)->NetworkThread(); }, this, "Network Thread");
    Thread renderer([](void* p) { ((Example*)p)->VulkanThread(); }, this, "Renderer Thread");
    Thread xr([](void* p) { ((Example*)p)->XRThread(); }, this, "XR Thread");

    while (platform.IsStillActive() && (audio.IsActive() || network.IsActive() || renderer.IsActive() || xr.IsActive())) {
        Thread::Yield();
    }

    audio.Join();
    network.Join();
    renderer.Join();
    xr.Join();
}
