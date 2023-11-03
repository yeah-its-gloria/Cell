// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Structures.hh"

#include <Cell/Shell/Controller/SwitchPro.hh>
#include <Cell/System/BlockImpl.hh>

namespace Cell::Shell::Controller {

Wrapped<SwitchPro*, Result> SwitchPro::Find() {
    const Wrapped<IO::HID::Device*, IO::Result> hidResult = IO::HID::Device::Open(0x057e, 0x2009);
    if (!hidResult.IsValid()) {
        switch (hidResult.Result()) {
        case IO::Result::NotFound: {
            return Result::NotFound;
        }

        default: {
            System::Panic("Cell::IO::HID::Device::Open failed");
        }
        }
    }

    SwitchPro* switchPro = new SwitchPro(hidResult.Unwrap());

    Result result = Result::Success;
    if (switchPro->device->GetConnectionType() == IO::HID::ConnectionType::USB) {
        result = switchPro->SubmitUSB((uint8_t)SwitchProUSBCommandId::Handshake);
        if (result != Result::Success) {
            delete switchPro;
            return result;
        }

        result = switchPro->SubmitUSB((uint8_t)SwitchProUSBCommandId::HighSpeed);
        if (result != Result::Success) {
            delete switchPro;
            return result;
        }

        result = switchPro->SubmitUSB((uint8_t)SwitchProUSBCommandId::Handshake);
        if (result != Result::Success) {
            delete switchPro;
            return result;
        }

        result = switchPro->SubmitUSB((uint8_t)SwitchProUSBCommandId::ForceUSB, false);
        if (result != Result::Success) {
            delete switchPro;
            return result;
        }
    }

    result = switchPro->SubmitCommand((uint8_t)SwitchProCommandId::EnableVibration, 0x01);
    if (result != Result::Success) {
        delete switchPro;
        return result;
    }

    result = switchPro->SubmitCommand((uint8_t)SwitchProCommandId::SetInputReportMode, 0x30);
    if (result != Result::Success) {
        delete switchPro;
        return result;
    }

    result = switchPro->SubmitCommand((uint8_t)SwitchProCommandId::SetPlayerLights, (uint8_t)SwitchProPlayerIndex::Player1);
    if (result != Result::Success) {
        delete switchPro;
        return result;
    }

    return switchPro;
}

SwitchPro::~SwitchPro() {
    delete this->device;
}

Result SwitchPro::Update() {
    this->lastReport = this->report;
    System::ClearMemory(this->report);

    SwitchProFullInputReport report = { };

    System::UnownedBlock reportRef { &report };
    IO::Result result = this->device->Read(reportRef, 33);
    switch (result) {
    case IO::Result::Success: {
        break;
    }

    case IO::Result::Timeout: {
        return Result::Timeout;
    }

    default: {
        System::Panic("Cell::IO::HID::Device::Read failed");
    }
    }

    CELL_ASSERT(report.reportId == SwitchProReportId::FullInput);

    SwitchProButton buttons = SWITCH_BUTTONS_U32(report);

    // TODO: allow flipping A/B and X/Y

    if ((buttons & SwitchProButton::A) == SwitchProButton::A) {
        this->report.buttons |= ControllerButton::A;
    }

    if ((buttons & SwitchProButton::B) == SwitchProButton::B) {
        this->report.buttons |= ControllerButton::B;
    }

    if ((buttons & SwitchProButton::X) == SwitchProButton::X) {
        this->report.buttons |= ControllerButton::X;
    }

    if ((buttons & SwitchProButton::Y) == SwitchProButton::Y) {
        this->report.buttons |= ControllerButton::Y;
    }

    if ((buttons & SwitchProButton::Plus) == SwitchProButton::Plus) {
        this->report.buttons |= ControllerButton::Menu;
    }

    if ((buttons & SwitchProButton::Minus) == SwitchProButton::Minus) {
        this->report.buttons |= ControllerButton::View;
    }

    if ((buttons & SwitchProButton::HOME) == SwitchProButton::HOME) {
        this->report.buttons |= ControllerButton::Guide;
    }

    if ((buttons & SwitchProButton::LeftStick) == SwitchProButton::LeftStick) {
        this->report.buttons |= ControllerButton::LeftStick;
    }

    if ((buttons & SwitchProButton::RightStick) == SwitchProButton::RightStick) {
        this->report.buttons |= ControllerButton::RightStick;
    }

    if ((buttons & SwitchProButton::L) == SwitchProButton::L) {
        this->report.buttons |= ControllerButton::LeftBumper;
    }

    if ((buttons & SwitchProButton::R) == SwitchProButton::R) {
        this->report.buttons |= ControllerButton::RightBumper;
    }

    if ((buttons & SwitchProButton::ZL) == SwitchProButton::ZL) {
        this->report.buttons |= ControllerButton::LeftTrigger;
    }

    if ((buttons & SwitchProButton::ZR) == SwitchProButton::ZR) {
        this->report.buttons |= ControllerButton::RightTrigger;
    }

    if ((buttons & SwitchProButton::Capture) == SwitchProButton::Capture) {
        this->report.buttons |= ControllerButton::Capture;
    }

    if ((buttons & SwitchProButton::DPadUp) == SwitchProButton::DPadUp) {
        this->report.buttons |= ControllerButton::DPadUp;
    }

    if ((buttons & SwitchProButton::DPadDown) == SwitchProButton::DPadDown) {
        this->report.buttons |= ControllerButton::DPadDown;
    }

    if ((buttons & SwitchProButton::DPadLeft) == SwitchProButton::DPadLeft) {
        this->report.buttons |= ControllerButton::DPadLeft;
    }

    if ((buttons & SwitchProButton::DPadRight) == SwitchProButton::DPadRight) {
        this->report.buttons |= ControllerButton::DPadRight;
    }

    uint16_t axisLX = ((report.leftStick[1] & 0xf) << 8) | report.leftStick[0];
    uint16_t axisLY = (report.leftStick[2] << 4) | ((report.leftStick[1] & 0xf0) >> 4);
    uint16_t axisRX = ((report.rightStick[1] & 0xf) << 8) | report.rightStick[0];
    uint16_t axisRY = (report.rightStick[2] << 4) | ((report.rightStick[1] & 0xf0) >> 4);

    this->report.leftStickX = (axisLX / 4096.0 - 0.5) * 2.0;
    this->report.leftStickY = (axisLY / 4096.0 - 0.5) * 2.0 * -1.0;
    this->report.rightStickX = (axisRX / 4096.0 - 0.5) * 2.0;
    this->report.rightStickY = (axisRY / 4096.0 - 0.5) * 2.0 * -1.0;

    this->report.leftTrigger = (buttons & SwitchProButton::ZL) == SwitchProButton::ZL ? 1.0 : 0.0;
    this->report.rightTrigger = (buttons & SwitchProButton::ZR) == SwitchProButton::ZR ? 1.0 : 0.0;

    return Result::Success;
}

}
