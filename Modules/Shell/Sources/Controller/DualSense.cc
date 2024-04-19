// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "DualSenseStructures.hh"

#include <Cell/Shell/Controller/DualSense.hh>
#include <Cell/Memory/Allocator.hh>
#include <Cell/Memory/UnownedBlock.hh>

namespace Cell::Shell::Controller {

Wrapped<DualSense*, Result> DualSense::Find() {
    Wrapped<IO::HID::Device*, IO::Result> hidResult = IO::HID::Device::Open(0x054c, 0x0ce6);
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

    return new DualSense(device);
}

DualSense::~DualSense() {
    delete this->device;
}

Result DualSense::Update() {
    this->lastReport = this->report;
    Memory::Clear(this->report);

    IO::Result result;

    // Update controller
    if (!this->hasUpdated) {
        DualSenseEffects effects = DualSenseEffects::SetLEDs | DualSenseEffects::SetPlayerLEDs;
        DualSenseEffectsEx effectsEx = DualSenseEffectsEx::None;

        if (this->properties.rumbleLeft > 0 || this->properties.rumbleRight > 0) {
            effects |= DualSenseEffects::NoAudioHaptics;
            effectsEx = DualSenseEffectsEx::ImprovedRumble;
        }

#pragma clang diagnostic ignored "-Wmissing-field-initializers"
        DualSenseEffectsPacket effectsPacket = {
            .id = 0x02,

            .effectToggles = effects,

            .rumbleRight = this->properties.rumbleRight,
            .rumbleLeft = this->properties.rumbleLeft,

            .effectTogglesEx = effectsEx,

            .ledBrightness = 0xff,

            .playerLEDs = DualSensePlayerLEDs::None,

            .ledRed = 0xdd,
            .ledGreen = 0xdd,
            .ledBlue = 0xdd,
        };

        switch (this->properties.playerIndex) {
        case 1: {
            effectsPacket.playerLEDs = DualSensePlayerLEDs::Player1;

            effectsPacket.ledRed = 0x00;
            effectsPacket.ledGreen = 0x00;
            effectsPacket.ledBlue = 0xfe;
            break;
        }

        case 2: {
            effectsPacket.playerLEDs = DualSensePlayerLEDs::Player2;

            effectsPacket.ledRed = 0xfe;
            effectsPacket.ledGreen = 0x00;
            effectsPacket.ledBlue = 0x00;
            break;
        }

        case 3: {
            effectsPacket.playerLEDs = DualSensePlayerLEDs::Player3;

            effectsPacket.ledRed = 0x00;
            effectsPacket.ledGreen = 0xfe;
            effectsPacket.ledBlue = 0x00;
            break;
        }

        case 4: {
            effectsPacket.playerLEDs = DualSensePlayerLEDs::Player4;

            effectsPacket.ledRed = 0xfe;
            effectsPacket.ledGreen = 0x00;
            effectsPacket.ledBlue = 0xfe;
            break;
        }

        default: {
            break;
        }
        }

        result = device->Write(Memory::UnownedBlock { &effectsPacket }, 33);
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

    DualSenseReportPacket packet;

    Memory::UnownedBlock packetRef { &packet };
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

    if ((packet.buttons & DualSenseButton::Cross) == DualSenseButton::Cross) {
        this->report.buttons |= ControllerButton::A;
    }

    if ((packet.buttons & DualSenseButton::Circle) == DualSenseButton::Circle) {
        this->report.buttons |= ControllerButton::B;
    }

    if ((packet.buttons & DualSenseButton::Square) == DualSenseButton::Square) {
        this->report.buttons |= ControllerButton::X;
    }

    if ((packet.buttons & DualSenseButton::Triangle) == DualSenseButton::Triangle) {
        this->report.buttons |= ControllerButton::Y;
    }

    if ((packet.buttons & DualSenseButton::Options) == DualSenseButton::Options) {
        this->report.buttons |= ControllerButton::Menu;
    }

    if ((packet.buttons & DualSenseButton::Create) == DualSenseButton::Create) {
        this->report.buttons |= ControllerButton::View;
    }

    if ((packet.buttons & DualSenseButton::PS) == DualSenseButton::PS) {
        this->report.buttons |= ControllerButton::Guide;
    }

    if ((packet.buttons & DualSenseButton::L3) == DualSenseButton::L3) {
        this->report.buttons |= ControllerButton::LeftStick;
    }

    if ((packet.buttons & DualSenseButton::R3) == DualSenseButton::R3) {
        this->report.buttons |= ControllerButton::RightStick;
    }

    if ((packet.buttons & DualSenseButton::L1) == DualSenseButton::L1) {
        this->report.buttons |= ControllerButton::LeftBumper;
    }

    if ((packet.buttons & DualSenseButton::R1) == DualSenseButton::R1) {
        this->report.buttons |= ControllerButton::RightBumper;
    }

    if ((packet.buttons & DualSenseButton::L2) == DualSenseButton::L2) {
        this->report.buttons |= ControllerButton::LeftTrigger;
    }

    if ((packet.buttons & DualSenseButton::R2) == DualSenseButton::R2) {
        this->report.buttons |= ControllerButton::RightTrigger;
    }

    if ((packet.buttons & DualSenseButton::Touchpad) == DualSenseButton::Touchpad) {
        this->report.buttons |= ControllerButton::Touchpad;
    }

    // Why Sony?
    switch ((uint32_t)(packet.buttons & DualSenseButton::DPadMask)) {
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

    // I apologize for the black magic at hand

    this->report.leftStickX = ((packet.leftStickX / 255.0 - 0.5) * 2.0) / 1.0;
    this->report.leftStickY = ((packet.leftStickY / 255.0 - 0.5) * 2.0) / 1.0;
    this->report.rightStickX = ((packet.rightStickX / 255.0 - 0.5) * 2.0) / 1.0;
    this->report.rightStickY = ((packet.rightStickY / 255.0 - 0.5) * 2.0) / 1.0;
    this->report.leftTrigger = packet.leftTrigger / 255.0;
    this->report.rightTrigger = packet.rightTrigger / 255.0;

    /*uint32_t touchData1 = ((uint32_t)packet.touchpadData1[2]) << 16 | ((uint32_t)packet.touchpadData1[1]) << 8 | packet.touchpadData1[0];
    uint32_t touchData2 = ((uint32_t)packet.touchpadData2[2]) << 16 | ((uint32_t)packet.touchpadData2[1]) << 8 | packet.touchpadData2[0];

    System::Log("% % % | % % %",
                (packet.touchpadCounter1 >>  0) & 0xff,
                ((touchData1) >> 12) & 0x0c, ((touchData1) >>  0) & 0x0c,
                (packet.touchpadCounter2 >> 24) & 0xff,
                ((touchData2) >> 12) & 0x0c, ((touchData2) >>  0) & 0x0c
    );*/

    return Result::Success;
}

}
