// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Editor.hh"

#include <Cell/System/Entry.hh>
#include <Cell/System/Thread.hh>

using namespace Cell;
using namespace Cell::System;

void CellEntry(Reference<String> parameterString) {
    Editor().Launch(parameterString.Unwrap());
}

Editor::~Editor() {
    delete this->shell;
}

void Editor::Launch(const String& parameterString) {
    (void)(parameterString);

    this->shell = Shell::CreateShell("Cell - Editor").Unwrap();

    while (true) {
        const Shell::Result result = this->shell->RunDispatch();
        if (result == Shell::Result::RequestedQuit) {
            break;
        }

        CELL_ASSERT(result == Shell::Result::Success);

        Thread::Yield();
    }
}
