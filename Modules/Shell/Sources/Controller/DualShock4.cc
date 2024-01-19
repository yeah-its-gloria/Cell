// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "DualShock4Structures.hh"

#include <Cell/Shell/Controller/DualShock4.hh>
#include <Cell/System/BlockImpl.hh>
#include <Cell/System/Log.hh>

namespace Cell::Shell::Controller {

Wrapped<DualShock4*, Result> DualShock4::Find() {
    const Wrapped<IO::HID::Device*, IO::Result> hidResult = IO::HID::Device::Open(0x054c, 0x05c4);
    if (!hidResult.IsValid()) {
        switch (hidResult.Result()) {
        case IO::Result::NotFound: {
            return Result::NotFound;
        }

        case IO::Result::Timeout: {
            return Result::Timeout;
        }

        default: {
            System::Panic("Cell::IO::HID::Device::Open failed");
        }
        }
    }

    IO::HID::Device* device = hidResult.Unwrap();
    if (device->GetConnectionType() != IO::HID::ConnectionType::USB) { // TODO: Bluetooth support
        return Result::NotFound;
    }

    System::OwnedBlock<uint8_t> packet(64);
    packet[0] = 0x12;
    IO::Result result = device->Read(packet, 33);
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

    return new DualShock4(device);
}

DualShock4::~DualShock4() {
    delete this->device;
}

Result DualShock4::Update() {
    this->lastReport = this->report;
    System::ClearMemory(this->report);

    IO::Result result;

    // Update controller
    if (!this->hasUpdated) {
        DualShock4EffectsPacket effectsPacket = {
            .id = 0x05,
            .magic = 0x07,

            .rumbleRight = this->properties.rumbleRight,
            .rumbleLeft = this->properties.rumbleLeft,

            .ledRed = 0xdd,
            .ledGreen = 0xdd,
            .ledBlue = 0xdd,
        };

        switch (this->properties.playerIndex) {
        case 1: {
            effectsPacket.ledRed = 0x00;
            effectsPacket.ledGreen = 0x00;
            effectsPacket.ledBlue = 0xfe;
            break;
        }

        case 2: {
            effectsPacket.ledRed = 0xfe;
            effectsPacket.ledGreen = 0x00;
            effectsPacket.ledBlue = 0x00;
            break;
        }

        case 3: {
            effectsPacket.ledRed = 0x00;
            effectsPacket.ledGreen = 0xfe;
            effectsPacket.ledBlue = 0x00;
            break;
        }

        case 4: {
            effectsPacket.ledRed = 0xfe;
            effectsPacket.ledGreen = 0x00;
            effectsPacket.ledBlue = 0xfe;
            break;
        }

        default: {
            break;
        }
        }

        result = device->Write(System::UnownedBlock { &effectsPacket }, 33);
        switch (result) {
        case IO::Result::Success: {
            break;
        }

        case IO::Result::Timeout: {
            return Result::Timeout;
        }

        default: {
            System::Panic("Cell::IO::HID::Device::Write failed");
        }
        }

        this->hasUpdated = true;
    }

    // Retrieve controller data

    DualShock4ReportPacket packet;

    System::UnownedBlock packetRef { &packet };
    result = device->Read(packetRef, 16);
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

    if (packet.id != 0x01) {
        return Result::InvalidReplies;
    }

    // TODO: find something cleaner than this

    DualShock4Button buttons = (DualShock4Button)(packet.buttonData[2] << 16 | packet.buttonData[1] << 8 | packet.buttonData[0]);

    if ((buttons & DualShock4Button::Cross) == DualShock4Button::Cross) {
        this->report.buttons |= ControllerButton::A;
    }

    if ((buttons & DualShock4Button::Circle) == DualShock4Button::Circle) {
        this->report.buttons |= ControllerButton::B;
    }

    if ((buttons & DualShock4Button::Square) == DualShock4Button::Square) {
        this->report.buttons |= ControllerButton::X;
    }

    if ((buttons & DualShock4Button::Triangle) == DualShock4Button::Triangle) {
        this->report.buttons |= ControllerButton::Y;
    }

    if ((buttons & DualShock4Button::Options) == DualShock4Button::Options) {
        this->report.buttons |= ControllerButton::Menu;
    }

    if ((buttons & DualShock4Button::Share) == DualShock4Button::Share) {
        this->report.buttons |= ControllerButton::View;
    }

    if ((buttons & DualShock4Button::PS) == DualShock4Button::PS) {
        this->report.buttons |= ControllerButton::Guide;
    }

    if ((buttons & DualShock4Button::L3) == DualShock4Button::L3) {
        this->report.buttons |= ControllerButton::LeftStick;
    }

    if ((buttons & DualShock4Button::R3) == DualShock4Button::R3) {
        this->report.buttons |= ControllerButton::RightStick;
    }

    if ((buttons & DualShock4Button::L1) == DualShock4Button::L1) {
        this->report.buttons |= ControllerButton::LeftBumper;
    }

    if ((buttons & DualShock4Button::R1) == DualShock4Button::R1) {
        this->report.buttons |= ControllerButton::RightBumper;
    }

    if ((buttons & DualShock4Button::L2) == DualShock4Button::L2) {
        this->report.buttons |= ControllerButton::LeftTrigger;
    }

    if ((buttons & DualShock4Button::R2) == DualShock4Button::R2) {
        this->report.buttons |= ControllerButton::RightTrigger;
    }

    if ((buttons & DualShock4Button::Touchpad) == DualShock4Button::Touchpad) {
        this->report.buttons |= ControllerButton::Touchpad;
    }

    // can you believe they did this shit twice and never thought it might look fucking stupid?
    switch ((uint32_t)(buttons & DualShock4Button::DPadMask)) {
    case 0x0: { // N
        this->report.buttons |= ControllerButton::DPadUp;
        break;
    }

    case 0x1: { // NE
        this->report.buttons |= ControllerButton::DPadUp | ControllerButton::DPadRight;
        break;
    }

    case 0x2: { // E
        this->report.buttons |= ControllerButton::DPadRight;
        break;
    }

    case 0x3: { // SE
        this->report.buttons |= ControllerButton::DPadDown | ControllerButton::DPadRight;
        break;
    }

    case 0x4: { // S
        this->report.buttons |= ControllerButton::DPadDown;
        break;
    }

    case 0x5: { // SW
        this->report.buttons |= ControllerButton::DPadDown | ControllerButton::DPadLeft;
        break;
    }

    case 0x6: { // W
        this->report.buttons |= ControllerButton::DPadLeft;
        break;
    }

    case 0x7: { // NW
        this->report.buttons |= ControllerButton::DPadUp | ControllerButton::DPadLeft;
        break;
    }

    default: {
        break;
    }
    }

    this->report.leftStickX = ((packet.leftStickX / 255.0 - 0.5) * 2.0) / 1.0;
    this->report.leftStickY = ((packet.leftStickY / 255.0 - 0.5) * 2.0) / 1.0;
    this->report.rightStickX = ((packet.rightStickX / 255.0 - 0.5) * 2.0) / 1.0;
    this->report.rightStickY = ((packet.rightStickY / 255.0 - 0.5) * 2.0) / 1.0;
    this->report.leftTrigger = packet.leftTrigger / 255.0;
    this->report.rightTrigger = packet.rightTrigger / 255.0;

    return Result::Success;
}

}
