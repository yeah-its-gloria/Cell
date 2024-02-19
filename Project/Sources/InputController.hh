// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "Example.hh"

#include <Cell/Mathematics/Matrix4x4.hh>
#include <Cell/System/Mutex.hh>

class InputController : public Cell::Object {
public:
    CELL_FUNCTION_INTERNAL static InputController* New(Example* owner);
    CELL_FUNCTION_INTERNAL ~InputController();

    CELL_FUNCTION_INTERNAL Cell::Mathematics::Matrix4x4 GetCamera();
    CELL_FUNCTION_INTERNAL bool TriggeredAudio();

private:
    CELL_INLINE InputController(Example* owner) : owner(owner) { }

    CELL_FUNCTION_INTERNAL void OnForward(const Cell::Shell::InputType type);
    CELL_FUNCTION_INTERNAL void OnBackward(const Cell::Shell::InputType type);
    CELL_FUNCTION_INTERNAL void OnRightward(const Cell::Shell::InputType type);
    CELL_FUNCTION_INTERNAL void OnLeftward(const Cell::Shell::InputType type);
    CELL_FUNCTION_INTERNAL void OnDownward(const Cell::Shell::InputType type);
    CELL_FUNCTION_INTERNAL void OnUpward(const Cell::Shell::InputType type);

    CELL_FUNCTION_INTERNAL void OnRun(const Cell::Shell::InputType type);

    CELL_FUNCTION_INTERNAL void OnForwardAxis(const double value);
    CELL_FUNCTION_INTERNAL void OnRightwardAxis(const double value);

    CELL_FUNCTION_INTERNAL void OnLookUpwardsMouse(const double value);
    CELL_FUNCTION_INTERNAL void OnLookRightwardsMouse(const double value);
    CELL_FUNCTION_INTERNAL void OnLookUpwardsController(const double value);
    CELL_FUNCTION_INTERNAL void OnLookRightwardsController(const double value);

    CELL_FUNCTION_INTERNAL void OnReset(const Cell::Shell::InputType type);
    CELL_FUNCTION_INTERNAL void OnBoing(const Cell::Shell::InputType type);
    CELL_FUNCTION_INTERNAL void OnFlipTextures(const Cell::Shell::InputType type);
    CELL_FUNCTION_INTERNAL void OnCloseTitle(const Cell::Shell::InputType type);

    Example* owner;
    Cell::System::Mutex inputMutex;

    Cell::Mathematics::Vector3 position { 0.0, 0.0, 0.0 };
    Cell::Mathematics::Vector3 velocity { 0.0, 0.0, 0.0 };

    double pitch = 0.0;
    double yaw = 0.0;

    bool isRunning = false;
    bool flipTextures = false;
    bool audioTriggered = false;

};
