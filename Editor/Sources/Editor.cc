// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Editor.hh"

#include <Cell/System/Thread.hh>

using namespace Cell;
using namespace Cell::System;

void CellEntry(Reference<IPlatform> platform, Reference<String> parameterString)
{
    Editor(platform.Unwrap()).Launch(parameterString.Unwrap());
}

void Editor::Launch(const String& parameterString)
{
    (void) (parameterString);

    this->platform.BlockUntilReady();

    while (platform.IsStillActive())
    {
        Thread::Yield();
    }
}
