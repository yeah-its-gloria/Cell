// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Reference.hh>
#include <Cell/Shell/Input.hh>
#include <Cell/Shell/Shell.hh>
#include <Cell/System/String.hh>

class Example : public Cell::Object {
public:
    CELL_INLINE Example() { }
    CELL_FUNCTION_INTERNAL ~Example();
    CELL_FUNCTION_INTERNAL void Launch(const Cell::System::String& parameterString);

    CELL_INLINE float GetDelta() const {
        return this->renderDeltaTime;
    }

    CELL_INLINE Cell::System::String GetContentPath(const Cell::System::String& string) {
        return Cell::System::String("./Projects/Example/Content") + string;
    }

private:
    CELL_FUNCTION_INTERNAL void AudioThread();
    CELL_FUNCTION_INTERNAL void NetworkThread();
    CELL_FUNCTION_INTERNAL void XRThread();
    CELL_FUNCTION_INTERNAL void VulkanThread();

    Cell::Shell::IShell* shell = nullptr;
    Cell::Shell::Input* input = nullptr;

    float elapsedTime = 0.f;
    float renderDeltaTime = 0.f;

    // TODO: communications :tm:
};
