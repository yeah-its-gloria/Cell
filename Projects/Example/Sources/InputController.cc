// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "InputController.hh"

#include <Cell/System/Log.hh>
#include <Cell/Utilities/MinMaxClamp.hh>
#include <Cell/Mathematics/Utilities.hh>

using namespace Cell;
using namespace Cell::Mathematics;
using namespace Cell::Mathematics::Utilities;
using namespace Cell::Shell;
using namespace Cell::Utilities;

#define BUTTON_ACTION(button, trigger) owner->shell->RegisterAction(button, [](const InputType type, void* inst) { ((InputController*)inst)->trigger(type); }, controller)
#define AXIS_ACTION(axis, trigger) owner->shell->RegisterAction(axis, [](const double value, void* inst) { ((InputController*)inst)->trigger(value); }, controller)

#define KEYBOARD_ACTION(button, trigger) BUTTON_ACTION(KeyboardButton::button, trigger)
#define CONTROLLER_ACTION(button, trigger) BUTTON_ACTION(ControllerButton::button, trigger)
#define MOUSE_ACTION(axis, trigger) AXIS_ACTION(MouseAxis::axis, trigger)
#define CONTROLLER_AXIS_ACTION(axis, trigger) AXIS_ACTION(ControllerAxis::axis, trigger)

const double GlobalSpeedMultiplier = 1.0;
const double MovementSpeed = 0.5;
const double MovementAcceleration = 5.0;
const double LookSpeed = 0.5;

InputController* InputController::New(Example* owner) {
    InputController* controller = new InputController(owner);

    KEYBOARD_ACTION(W, OnForward);
    KEYBOARD_ACTION(S, OnBackward);
    KEYBOARD_ACTION(D, OnRightward);
    KEYBOARD_ACTION(A, OnLeftward);
    KEYBOARD_ACTION(E, OnDownward);
    KEYBOARD_ACTION(Q, OnUpward);
    KEYBOARD_ACTION(LeftShift, OnRun);

    CONTROLLER_ACTION(DPadUp, OnForward);
    CONTROLLER_ACTION(DPadDown, OnBackward);
    CONTROLLER_ACTION(DPadLeft, OnLeftward);
    CONTROLLER_ACTION(DPadRight, OnRightward);
    CONTROLLER_ACTION(Y, OnUpward);
    CONTROLLER_ACTION(B, OnDownward);
    CONTROLLER_ACTION(X, OnReset);
    CONTROLLER_ACTION(A, OnBoing);
    CONTROLLER_ACTION(RightBumper, OnRun);

    CONTROLLER_AXIS_ACTION(LeftStickY, OnForwardAxis);
    CONTROLLER_AXIS_ACTION(LeftStickX, OnRightwardAxis);

    MOUSE_ACTION(PositionY, OnLookUpwardsMouse);
    MOUSE_ACTION(PositionX, OnLookRightwardsMouse);

    CONTROLLER_AXIS_ACTION(RightStickY, OnLookUpwardsController);
    CONTROLLER_AXIS_ACTION(RightStickX, OnLookRightwardsController);

    KEYBOARD_ACTION(R, OnReset);
    KEYBOARD_ACTION(F, OnBoing);
    KEYBOARD_ACTION(Tab, OnFlipTextures);
    KEYBOARD_ACTION(Escape, OnCloseTitle);

    return controller;
}

InputController::~InputController() { }

Matrix4x4 InputController::GetCamera() {
    this->inputMutex.Lock();

    Matrix4x4 rotation = Matrix4x4::FromQuaternion(Quaternion::FromRotation(DegreesToRadians(this->pitch), { 0.0, -1.0, 0.0 }))
                            .Multiply(Matrix4x4::FromQuaternion(Quaternion::FromRotation(DegreesToRadians(this->yaw), { -1.0, 0.0, 0.0 })));

    this->position += (this->velocity * Vector3(1.0, -1.0, -1.0)).Rotate(rotation);

    Matrix4x4 camera = Matrix4x4::FromIdentity().Translate(this->position).Multiply(rotation).Invert();

    this->inputMutex.Unlock();
    return camera;
}

bool InputController::TriggeredAudio() {
    if (this->audioTriggered) {
        this->audioTriggered = false;
        return true;
    }

    return false;
}

void InputController::OnForward(const InputType type) {
    this->inputMutex.Lock();
    this->velocity.z = type != InputType::Released ? MovementSpeed * (this->isRunning ? MovementAcceleration : 1.0) * this->owner->shellDeltaTime * GlobalSpeedMultiplier : 0;
    this->inputMutex.Unlock();
}

void InputController::OnBackward(const InputType type) {
    this->inputMutex.Lock();
    this->velocity.z = type != InputType::Released ? -MovementSpeed * (this->isRunning ? MovementAcceleration : 1.0) * this->owner->shellDeltaTime * GlobalSpeedMultiplier : 0;
    this->inputMutex.Unlock();
}

void InputController::OnRightward(const InputType type) {
    this->inputMutex.Lock();
    this->velocity.x = type != InputType::Released ? MovementSpeed * (this->isRunning ? MovementAcceleration : 1.0) * this->owner->shellDeltaTime * GlobalSpeedMultiplier : 0;
    this->inputMutex.Unlock();
}

void InputController::OnLeftward(const InputType type) {
    this->inputMutex.Lock();
    this->velocity.x = type != InputType::Released ? -MovementSpeed * (this->isRunning ? MovementAcceleration : 1.0) * this->owner->shellDeltaTime * GlobalSpeedMultiplier : 0;
    this->inputMutex.Unlock();
}

void InputController::OnDownward(const InputType type) {
    this->inputMutex.Lock();
    this->velocity.y = type != InputType::Released ? MovementSpeed * (this->isRunning ? MovementAcceleration : 1.0) * this->owner->shellDeltaTime * GlobalSpeedMultiplier : 0;
    this->inputMutex.Unlock();
}

void InputController::OnUpward(const InputType type) {
    this->inputMutex.Lock();
    this->velocity.y = type != InputType::Released ? -MovementSpeed * (this->isRunning ? MovementAcceleration : 1.0) * this->owner->shellDeltaTime * GlobalSpeedMultiplier : 0;
    this->inputMutex.Unlock();
}

void InputController::OnRun(const InputType type) {
    this->inputMutex.Lock();
    this->isRunning = type != InputType::Released;
    this->inputMutex.Unlock();
}

void InputController::OnForwardAxis(const double value) {
    this->inputMutex.Lock();
    this->velocity.z = -MovementSpeed * value * this->owner->shellDeltaTime * GlobalSpeedMultiplier;
    this->inputMutex.Unlock();
}

void InputController::OnRightwardAxis(const double value) {
    this->inputMutex.Lock();
    this->velocity.x = MovementSpeed * value * this->owner->shellDeltaTime * GlobalSpeedMultiplier;
    this->inputMutex.Unlock();
}

void InputController::OnLookUpwardsMouse(const double value) {
    this->inputMutex.Lock();
    this->yaw = Clamp(this->yaw + (value * LookSpeed) * GlobalSpeedMultiplier, -45.0, 45.0);
    this->inputMutex.Unlock();
}

void InputController::OnLookRightwardsMouse(const double value) {
    this->inputMutex.Lock();
    this->pitch += (value * LookSpeed) * GlobalSpeedMultiplier;
    this->inputMutex.Unlock();
}

void InputController::OnLookUpwardsController(const double value) {
    (void)(value);
}

void InputController::OnLookRightwardsController(const double value) {
    (void)(value);
}

void InputController::OnReset(const InputType type) {
    if (type != InputType::Pressed) {
        return;
    }

    this->inputMutex.Lock();
    this->position = 0;
    this->pitch = 0;
    this->yaw = 0;
    this->inputMutex.Unlock();
}

void InputController::OnBoing(const InputType type) {
    if (type != InputType::Pressed) {
        return;
    }

    this->audioTriggered = true;
}

void InputController::OnFlipTextures(const InputType type) {
    if (type != InputType::Pressed) {
        return;
    }

    this->flipTextures = !this->flipTextures;
}

void InputController::OnCloseTitle(const InputType type) {
    if (type != InputType::Pressed) {
        return;
    }

    this->owner->shell->RequestQuit();
}
