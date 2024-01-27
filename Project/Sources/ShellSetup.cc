// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Example.hh"

#include <math.h>

using namespace Cell;
using namespace Cell::Shell;

const float GlobalSpeedMultiplier = 0.01f;
const float MovementSpeed = 1.f;

void Example::OnForward(const InputType type) {
    if (type == InputType::Released) {
        return;
    }

    this->inputMutex.Lock();
    this->position.z += MovementSpeed * this->shellDeltaTime * GlobalSpeedMultiplier;
    this->inputMutex.Unlock();
}

void Example::OnBackward(const InputType type) {
    if (type == InputType::Released) {
        return;
    }

    this->inputMutex.Lock();
    this->position.z -= MovementSpeed * this->shellDeltaTime * GlobalSpeedMultiplier;
    this->inputMutex.Unlock();
}

void Example::OnForwardAxis(const double value) {
    this->inputMutex.Lock();
    this->position.z -= MovementSpeed * value * this->shellDeltaTime * GlobalSpeedMultiplier;
    this->inputMutex.Unlock();
}

void Example::OnRightwardAxis(const double value) {
    this->inputMutex.Lock();
    this->position.x -= MovementSpeed * value * this->shellDeltaTime * GlobalSpeedMultiplier;
    this->inputMutex.Unlock();
}

void Example::OnLeftward(const InputType type) {
    if (type == InputType::Released) {
        return;
    }

    this->inputMutex.Lock();
    this->position.x += MovementSpeed * this->shellDeltaTime * GlobalSpeedMultiplier;
    this->inputMutex.Unlock();
}

void Example::OnRightward(const InputType type) {
    if (type == InputType::Released) {
        return;
    }

    this->inputMutex.Lock();
    this->position.x -= MovementSpeed * this->shellDeltaTime * GlobalSpeedMultiplier;
    this->inputMutex.Unlock();
}

void Example::OnUpward(const InputType type) {
    if (type == InputType::Released) {
        return;
    }

    this->inputMutex.Lock();
    this->position.y += MovementSpeed * this->shellDeltaTime * GlobalSpeedMultiplier;
    this->inputMutex.Unlock();
}

void Example::OnDownward(const InputType type) {
    if (type == InputType::Released) {
        return;
    }

    this->inputMutex.Lock();
    this->position.y -= MovementSpeed * this->shellDeltaTime * GlobalSpeedMultiplier;
    this->inputMutex.Unlock();
}

void Example::OnReset(const InputType type) {
    if (type != InputType::Pressed) {
        return;
    }

    this->inputMutex.Lock();
    this->position = 0;
    this->rotationX = 0;
    this->rotationY = 0;
    this->inputMutex.Unlock();
}

void Example::OnBoing(const InputType type) {
    if (type != InputType::Pressed) {
        return;
    }

    this->Trigger();
}


void Example::OnFlipTextures(const InputType type) {
    if (type != InputType::Pressed) {
        return;
    }

    this->flipTexture = !this->flipTexture;
}

void Example::OnCloseTitle(const InputType type) {
    if (type != InputType::Pressed) {
        return;
    }

    this->shell->RequestQuit();
}

void Example::TurnCameraXController(const double value) {
    this->inputMutex.Lock();
    this->rotationX += fmod(10.f * value, 360.0);
    this->inputMutex.Unlock();
}

void Example::TurnCameraYController(const double value) {
    this->inputMutex.Lock();
    this->rotationY -= fmod(10.f * value, 360.0);
    this->inputMutex.Unlock();
}

void Example::ShellSetup() {
    this->shell = Shell::CreateShell("Cell - Hi Aurelia").Unwrap();

    Result shellResult = this->shell->DiscoverPeripherals();
    CELL_ASSERT(shellResult == Result::Success);

    // Keyboard

    shellResult = this->shell->RegisterAction(KeyboardButton::W, [](const InputType type, void* inst) { ((Example*)inst)->OnForward(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = this->shell->RegisterAction(KeyboardButton::S, [](const InputType type, void* inst) { ((Example*)inst)->OnBackward(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = this->shell->RegisterAction(KeyboardButton::A, [](const InputType type, void* inst) { ((Example*)inst)->OnLeftward(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = this->shell->RegisterAction(KeyboardButton::D, [](const InputType type, void* inst) { ((Example*)inst)->OnRightward(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = this->shell->RegisterAction(KeyboardButton::Q, [](const InputType type, void* inst) { ((Example*)inst)->OnUpward(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = this->shell->RegisterAction(KeyboardButton::E, [](const InputType type, void* inst) { ((Example*)inst)->OnDownward(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = this->shell->RegisterAction(KeyboardButton::R, [](const InputType type, void* inst) { ((Example*)inst)->OnReset(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = this->shell->RegisterAction(KeyboardButton::F, [](const InputType type, void* inst) { ((Example*)inst)->OnBoing(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = this->shell->RegisterAction(KeyboardButton::Tab, [](const InputType type, void* inst) { ((Example*)inst)->OnFlipTextures(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = this->shell->RegisterAction(KeyboardButton::Escape, [](const InputType type, void* inst) { ((Example*)inst)->OnCloseTitle(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    // Controller

    shellResult = this->shell->RegisterAction(ControllerButton::DPadUp, [](const InputType type, void* inst) { ((Example*)inst)->OnForward(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = this->shell->RegisterAction(ControllerButton::DPadDown, [](const InputType type, void* inst) { ((Example*)inst)->OnBackward(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = this->shell->RegisterAction(ControllerButton::DPadLeft, [](const InputType type, void* inst) { ((Example*)inst)->OnLeftward(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = this->shell->RegisterAction(ControllerButton::DPadRight, [](const InputType type, void* inst) { ((Example*)inst)->OnRightward(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = this->shell->RegisterAction(ControllerButton::Y, [](const InputType type, void* inst) { ((Example*)inst)->OnUpward(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = this->shell->RegisterAction(ControllerButton::B, [](const InputType type, void* inst) { ((Example*)inst)->OnDownward(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = this->shell->RegisterAction(ControllerButton::X, [](const InputType type, void* inst) { ((Example*)inst)->OnReset(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = this->shell->RegisterAction(ControllerButton::A, [](const InputType type, void* inst) { ((Example*)inst)->OnBoing(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = this->shell->RegisterAction(ControllerAxis::LeftStickX, [](const double value, void* inst) { ((Example*)inst)->OnRightwardAxis(value); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = this->shell->RegisterAction(ControllerAxis::LeftStickY, [](const double value, void* inst) { ((Example*)inst)->OnForwardAxis(value); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = this->shell->RegisterAction(ControllerAxis::RightStickX, [](const double value, void* inst) { ((Example*)inst)->TurnCameraXController(value); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = this->shell->RegisterAction(ControllerAxis::RightStickY, [](const double value, void* inst) { ((Example*)inst)->TurnCameraYController(value); }, this);
    CELL_ASSERT(shellResult == Result::Success);
}
