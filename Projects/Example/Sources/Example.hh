// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Mathematics/Vector3.hh>
#include <Cell/Shell/Shell.hh>
#include <Cell/System/Thread.hh>

class Example : public Cell::Object {
friend class InputController;

public:
    CELL_FUNCTION_INTERNAL Example() = default;
    CELL_FUNCTION_INTERNAL ~Example();
    CELL_FUNCTION_INTERNAL void Launch(const Cell::String& parameterString);

    CELL_FUNCTION_INTERNAL inline Cell::String GetContentPath(const Cell::String& string) {
        return Cell::String("./Projects/Example/Content") + string;
    }

private:
    CELL_FUNCTION_INTERNAL void ShellSetup();

    CELL_FUNCTION_INTERNAL void AudioThread();
    CELL_FUNCTION_INTERNAL void RendererThread();
    CELL_FUNCTION_INTERNAL void XRThread();

    Cell::Shell::IShell* shell = nullptr;
    class InputController* controller = nullptr;

    // milliseconds, guaranteed to be at least 0.001

    double renderDeltaTime = 0.f;
    double shellDeltaTime = 0.f;
};
