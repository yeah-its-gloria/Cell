// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Mathematics/Vector3.hh>
#include <Cell/Shell/Input.hh>
#include <Cell/System/Channel.hh>
#include <Cell/System/Thread.hh>

class Example : public Cell::Object {
public:
    CELL_INLINE Example() { }
    CELL_FUNCTION_INTERNAL ~Example();
    CELL_FUNCTION_INTERNAL void Launch(const Cell::System::String& parameterString);

    CELL_INLINE void Trigger() {
        this->audioTrigger.TrySend(1);
    }

    CELL_INLINE Cell::System::String GetContentPath(const Cell::System::String& string) {
        return Cell::System::String("./Project/Content") + string;
    }

private:
    CELL_FUNCTION_INTERNAL void AudioThread();
    CELL_FUNCTION_INTERNAL void InputThread();
    CELL_FUNCTION_INTERNAL void NetworkThread();
    CELL_FUNCTION_INTERNAL void XRThread();
    CELL_FUNCTION_INTERNAL void VulkanThread();

    CELL_FUNCTION_INTERNAL void OnForward(const Cell::Shell::InputType type);
    CELL_FUNCTION_INTERNAL void OnBackward(const Cell::Shell::InputType type);
    CELL_FUNCTION_INTERNAL void OnForwardAxis(const double value);
    CELL_FUNCTION_INTERNAL void OnRightwardAxis(const double value);
    CELL_FUNCTION_INTERNAL void OnLeftward(const Cell::Shell::InputType type);
    CELL_FUNCTION_INTERNAL void OnRightward(const Cell::Shell::InputType type);
    CELL_FUNCTION_INTERNAL void OnUpward(const Cell::Shell::InputType type);
    CELL_FUNCTION_INTERNAL void OnDownward(const Cell::Shell::InputType type);
    CELL_FUNCTION_INTERNAL void OnReset(const Cell::Shell::InputType type);
    CELL_FUNCTION_INTERNAL void OnBoing(const Cell::Shell::InputType type);

    Cell::Shell::IShell* shell = nullptr;
    Cell::Shell::Input* input = nullptr;

    float elapsedTime = 0.f;
    float renderDeltaTime = 0.f;

    Cell::System::Mutex inputMutex;
    Cell::Mathematics::Vector3 position { 0.f, 0.f, 0.f };

    Cell::System::Channel<uint8_t> audioTrigger { 0 };
};
