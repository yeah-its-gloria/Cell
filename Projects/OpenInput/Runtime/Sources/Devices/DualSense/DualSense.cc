// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "../DualSense.hh"

#include <Cell/System/BlockImpl.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Thread.hh>

using namespace Cell;

namespace Runtime::Devices {

struct PS5Packet {
    uint8_t id;
    uint8_t leftStickX;
    uint8_t leftStickY;
    uint8_t rightStickX;
    uint8_t rightStickY;
    uint8_t buttons;
    uint8_t counter;
    uint8_t triggerLeft;
    uint8_t triggerRight;
};

Wrapped<DualSense*, Result> DualSense::Find() {
    const Wrapped<IO::HID*, IO::Result> hidResult = IO::HID::Open(0x054c, 0x0ce6);
    if (!hidResult.IsValid()) {
        switch (hidResult.Result()) {
        case IO::Result::NotFound: {
            return Result::NotFound;
        }

        case IO::Result::Timeout: {
            return Result::Timeout;
        }

        default: {
            System::Panic("Cell::IO::HID::Open failed");
        }
        }
    }

    IO::HID* device = hidResult.Unwrap();

    System::ManagedBlock<uint8_t> data(78);
    while (true) {
        IO::Result result = device->Read(data, 500);
        switch (result) {
        case IO::Result::Success:
        case IO::Result::Incomplete: {
            break;
        }

        default: {
            System::Panic("Cell::IO::HID::Read failed");
        }
        }

        PS5Packet* packet = (PS5Packet*)data.Pointer();
        System::Log("%d", packet->buttons);

        System::Thread::Yield();
    }

    return Result::Success;
}

DualSense::~DualSense() {
    delete this->device;
}

Result DualSense::Poll() {
    return Result::Success;
}

}
