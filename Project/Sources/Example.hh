// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Mathematics/Vector3.hh>
#include <Cell/Shell/Shell.hh>
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
    CELL_FUNCTION_INTERNAL void OnFlipTextures(const Cell::Shell::InputType type);
    CELL_FUNCTION_INTERNAL void OnCloseTitle(const Cell::Shell::InputType type);

    CELL_FUNCTION_INTERNAL void TurnCameraXController(const double value);
    CELL_FUNCTION_INTERNAL void TurnCameraYController(const double value);

    Cell::Shell::IShell* shell = nullptr;

    // milliseconds
    double elapsedTime = 0.f;

    // milliseconds, guaranteed to be at least 0.001
    double renderDeltaTime = 0.f;

    Cell::System::Mutex inputMutex;
    Cell::Mathematics::Vector3 position;
    double rotationX = 0.f;
    double rotationY = 0.f;
    bool flipTexture = true;

    Cell::System::Channel<uint8_t> audioTrigger { 0 };
};
