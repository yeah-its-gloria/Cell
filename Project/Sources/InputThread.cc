// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Example.hh"

#include <Cell/Scoped.hh>
#include <Cell/Shell/Input.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Timer.hh>
#include <Cell/Utilities/MinMaxClamp.hh>

#include <math.h>

using namespace Cell;
using namespace Cell::Shell;

const float MovementSpeed = 15.f;

void Example::OnForward(const InputType type) {
    if (type == InputType::Released) {
        return;
    }

    this->renderDeltaTime = 0.001;
    this->inputMutex.Lock();
    this->position.z += MovementSpeed * this->renderDeltaTime;
    this->inputMutex.Unlock();
}

void Example::OnBackward(const InputType type) {
    if (type == InputType::Released) {
        return;
    }

    this->renderDeltaTime = 0.001;
    this->inputMutex.Lock();
    this->position.z -= MovementSpeed * this->renderDeltaTime;
    this->inputMutex.Unlock();
}

void Example::OnForwardAxis(const double value) {
    this->renderDeltaTime = 0.001;
    this->inputMutex.Lock();
    this->position.z -= MovementSpeed * this->renderDeltaTime * value;
    this->inputMutex.Unlock();
}

void Example::OnRightwardAxis(const double value) {
    this->renderDeltaTime = 0.001;
    this->inputMutex.Lock();
    this->position.x -= MovementSpeed * this->renderDeltaTime * value;
    this->inputMutex.Unlock();
}

void Example::OnLeftward(const InputType type) {
    if (type == InputType::Released) {
        return;
    }

    this->renderDeltaTime = 0.001;
    this->inputMutex.Lock();
    this->position.x += MovementSpeed * this->renderDeltaTime;
    this->inputMutex.Unlock();
}

void Example::OnRightward(const InputType type) {
    if (type == InputType::Released) {
        return;
    }

    this->renderDeltaTime = 0.001;
    this->inputMutex.Lock();
    this->position.x -= MovementSpeed * this->renderDeltaTime;
    this->inputMutex.Unlock();
}

void Example::OnUpward(const InputType type) {
    if (type == InputType::Released) {
        return;
    }

    this->renderDeltaTime = 0.001;
    this->inputMutex.Lock();
    this->position.y -= MovementSpeed * this->renderDeltaTime;
    this->inputMutex.Unlock();
}

void Example::OnDownward(const InputType type) {
    if (type == InputType::Released) {
        return;
    }

    this->renderDeltaTime = 0.001;
    this->inputMutex.Lock();
    this->position.y += MovementSpeed * this->renderDeltaTime;
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
    this->renderDeltaTime = 0.001;
    this->inputMutex.Lock();
    this->rotationX += fmod(10.f * this->renderDeltaTime * value, 360.0);
    this->inputMutex.Unlock();
}

void Example::TurnCameraYController(const double value) {
    this->renderDeltaTime = 0.001;
    this->inputMutex.Lock();
    this->rotationY -= fmod(10.f * this->renderDeltaTime * value, 360.0);
    this->inputMutex.Unlock();
}

void Example::InputThread() {
    ScopedObject input = this->shell->CreateInputHandler();

    Result shellResult = input->DiscoverControllers();
    CELL_ASSERT(shellResult == Result::Success);
    // Keyboard

    shellResult = input->RegisterAction(KeyboardButton::W, [](const InputType type, void* inst) { ((Example*)inst)->OnForward(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(KeyboardButton::S, [](const InputType type, void* inst) { ((Example*)inst)->OnBackward(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(KeyboardButton::A, [](const InputType type, void* inst) { ((Example*)inst)->OnLeftward(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(KeyboardButton::D, [](const InputType type, void* inst) { ((Example*)inst)->OnRightward(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(KeyboardButton::Q, [](const InputType type, void* inst) { ((Example*)inst)->OnUpward(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(KeyboardButton::E, [](const InputType type, void* inst) { ((Example*)inst)->OnDownward(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(KeyboardButton::R, [](const InputType type, void* inst) { ((Example*)inst)->OnReset(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(KeyboardButton::F, [](const InputType type, void* inst) { ((Example*)inst)->OnBoing(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(KeyboardButton::Tab, [](const InputType type, void* inst) { ((Example*)inst)->OnFlipTextures(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(KeyboardButton::Escape, [](const InputType type, void* inst) { ((Example*)inst)->OnCloseTitle(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    // Controller

    shellResult = input->RegisterAction(ControllerButton::DPadUp, [](const InputType type, void* inst) { ((Example*)inst)->OnForward(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(ControllerButton::DPadDown, [](const InputType type, void* inst) { ((Example*)inst)->OnBackward(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(ControllerButton::DPadLeft, [](const InputType type, void* inst) { ((Example*)inst)->OnLeftward(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(ControllerButton::DPadRight, [](const InputType type, void* inst) { ((Example*)inst)->OnRightward(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(ControllerButton::Y, [](const InputType type, void* inst) { ((Example*)inst)->OnUpward(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(ControllerButton::B, [](const InputType type, void* inst) { ((Example*)inst)->OnDownward(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(ControllerButton::X, [](const InputType type, void* inst) { ((Example*)inst)->OnReset(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(ControllerButton::A, [](const InputType type, void* inst) { ((Example*)inst)->OnBoing(type); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(ControllerAxis::LeftStickX, [](const double value, void* inst) { ((Example*)inst)->OnRightwardAxis(value); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(ControllerAxis::LeftStickY, [](const double value, void* inst) { ((Example*)inst)->OnForwardAxis(value); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(ControllerAxis::RightStickX, [](const double value, void* inst) { ((Example*)inst)->TurnCameraXController(value); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(ControllerAxis::RightStickY, [](const double value, void* inst) { ((Example*)inst)->TurnCameraYController(value); }, this);
    CELL_ASSERT(shellResult == Result::Success);

    // Polling

    while (this->shell->IsStillActive()) {
        shellResult = input->Poll();
        CELL_ASSERT(shellResult == Shell::Result::Success);

        System::Thread::Yield();
    }
}
