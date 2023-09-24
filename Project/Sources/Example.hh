// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Reference.hh>
#include <Cell/Shell/Input.hh>
#include <Cell/Shell/Shell.hh>
#include <Cell/System/Channel.hh>
#include <Cell/System/String.hh>

class Example : public Cell::Object {
public:
    CELL_INLINE Example() { }
    CELL_FUNCTION_INTERNAL ~Example();
    CELL_FUNCTION_INTERNAL void Launch(const Cell::System::String& parameterString);

    CELL_INLINE float GetDelta() {
        return this->renderDeltaTime;
    }

    CELL_FUNCTION void Trigger() {
        this->audioTrigger.TrySend(1);
    }

    CELL_INLINE Cell::System::String GetContentPath(const Cell::System::String& string) {
        return Cell::System::String("./Project/Content") + string;
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

    Cell::System::Channel<uint8_t> audioTrigger { 0 };
};
