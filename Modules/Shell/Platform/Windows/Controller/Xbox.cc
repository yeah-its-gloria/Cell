// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Controller/Xbox.hh>
#include <Cell/System/Log.hh>
#include <Cell/Memory/Allocator.hh>
#include <Cell/System/Timer.hh>

#include <wrl.h>
#include <windows.gaming.input.h>

namespace Cell::Shell::Controller {

using namespace ABI::Windows::Foundation::Collections;
using namespace ABI::Windows::Gaming::Input;
using namespace Microsoft::WRL::Wrappers;

struct XboxData {
    IGamepadStatics* statics;
    IGamepad* gamepad;
};

Wrapped<Xbox*, Result> Xbox::Find() {
    IGamepadStatics* statics = nullptr;
    HRESULT result = RoGetActivationFactory(HStringReference(RuntimeClass_Windows_Gaming_Input_Gamepad).Get(), __uuidof(IGamepadStatics), (void**)&statics);
    CELL_ASSERT(SUCCEEDED(result));

    System::Sleep(500);

    IVectorView<Gamepad*>* gamepads = nullptr;
    result = statics->get_Gamepads(&gamepads);
    CELL_ASSERT(SUCCEEDED(result));

    uint32_t size = 0;
    result = gamepads->get_Size(&size);
    CELL_ASSERT(SUCCEEDED(result));

    if (size == 0) {
        return Result::NotFound;
    }

    IGamepad* gamepad = nullptr;
    gamepads->GetAt(0, &gamepad);

    XboxData* data = Memory::Allocate<XboxData>();

    data->statics = statics;
    data->gamepad = gamepad;

    return new Xbox((uintptr_t)data);
}

Xbox::~Xbox() {
    XboxData* data = (XboxData*)this->device;

    data->gamepad->Release();
    data->statics->Release();

    Memory::Free(data);
}

Result Xbox::Update() {
    this->lastReport = this->report;
    Memory::Clear(this->report);

    XboxData* data = (XboxData*)this->device;

    GamepadReading reading = { };
    const HRESULT result = data->gamepad->GetCurrentReading(&reading);
    CELL_ASSERT(SUCCEEDED(result));

    if ((reading.Buttons & GamepadButtons_A)) {
        this->report.buttons |= ControllerButton::A;
    }

    if ((reading.Buttons & GamepadButtons_B)) {
        this->report.buttons |= ControllerButton::B;
    }

    if ((reading.Buttons & GamepadButtons_X)) {
        this->report.buttons |= ControllerButton::X;
    }

    if ((reading.Buttons & GamepadButtons_Y)) {
        this->report.buttons |= ControllerButton::Y;
    }

    if ((reading.Buttons & GamepadButtons_Menu)) {
        this->report.buttons |= ControllerButton::Menu;
    }

    if ((reading.Buttons & GamepadButtons_View)) {
        this->report.buttons |= ControllerButton::View;
    }

    if ((reading.Buttons & GamepadButtons_LeftThumbstick)) {
        this->report.buttons |= ControllerButton::LeftStick;
    }

    if ((reading.Buttons & GamepadButtons_RightThumbstick)) {
        this->report.buttons |= ControllerButton::RightStick;
    }

    if ((reading.Buttons & GamepadButtons_LeftShoulder)) {
        this->report.buttons |= ControllerButton::LeftBumper;
    }

    if ((reading.Buttons & GamepadButtons_RightShoulder)) {
        this->report.buttons |= ControllerButton::RightBumper;
    }

    if (reading.LeftTrigger > 0.9) {
        this->report.buttons |= ControllerButton::LeftTrigger;
    }

    if (reading.RightTrigger > 0.9) {
        this->report.buttons |= ControllerButton::RightTrigger;
    }

    if ((reading.Buttons & GamepadButtons_DPadUp)) {
        this->report.buttons |= ControllerButton::DPadUp;
    }

    if ((reading.Buttons & GamepadButtons_DPadDown)) {
        this->report.buttons |= ControllerButton::DPadDown;
    }

    if ((reading.Buttons & GamepadButtons_DPadLeft)) {
        this->report.buttons |= ControllerButton::DPadLeft;
    }

    if ((reading.Buttons & GamepadButtons_DPadRight)) {
        this->report.buttons |= ControllerButton::DPadRight;
    }

    // Xbox sticks are Y+ up, we want the opposite

    this->report.leftStickX = reading.LeftThumbstickX;
    this->report.leftStickY = reading.LeftThumbstickY * -1.0;
    this->report.rightStickX = reading.RightThumbstickX;
    this->report.rightStickY = reading.RightThumbstickY * -1.0;
    this->report.leftTrigger = reading.LeftTrigger;
    this->report.rightTrigger = reading.RightTrigger;

    return Result::Success;
}

}
