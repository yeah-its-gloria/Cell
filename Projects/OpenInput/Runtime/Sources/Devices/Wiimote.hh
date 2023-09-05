// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "Result.hh"
#include "Wiimote/Communications/LED.hh"
#include "Wiimote/Communications/Reports.hh"

#include <Cell/IO/HID.hh>

namespace Runtime::Devices {

enum class WiimoteExtensionType : uint8_t {
    None,
    Nunchuck,
    ClassicController,
    ClassicControllerPro,

    Unknown = 0xff
};

class Wiimote : public Cell::Object {
public:
    // Tries to find an uninitialized Wiimote and initializes it.
    CELL_FUNCTION_INTERNAL static Cell::Wrapped<Wiimote*, Result> Find();

    // Destructs the Wiimote instance.
    CELL_FUNCTION_INTERNAL ~Wiimote();

    // Polls the Wiimote.
    CELL_FUNCTION_INTERNAL Result Poll();

    // Sets the LEDs of the Wiimote. Also allows toggling rumble.
    CELL_FUNCTION_INTERNAL Result SetLEDs(WiimoteCommunications::LEDMask mask);

    // Sets the rumble state.
    CELL_FUNCTION_INTERNAL Result SetRumble(const bool enable);

    // Returns the current status of this Wiimote.
    CELL_NODISCARD CELL_INLINE WiimoteCommunications::StatusReport GetStatus() const {
        return this->currentStatus;
    }

    // Returns the current input data of this Wiimote.
    CELL_NODISCARD CELL_INLINE WiimoteCommunications::ButtonsAccelExtensionReport GetInputData() const {
        return this->currentInputData;
    }

    // Returns the currently connected extension type.
    CELL_NODISCARD CELL_INLINE WiimoteExtensionType GetExtensionType() const {
        return this->extensionType;
    }

private:
    CELL_INLINE Wiimote(Cell::IO::HID* device, WiimoteCommunications::StatusReport status)
        : device(device), currentStatus(status), currentInputData {}, extensionType(WiimoteExtensionType::None) {}

    CELL_FUNCTION_INTERNAL Result SetUpReporting();
    CELL_FUNCTION_INTERNAL Result ReceiveAcknowledgement();

    Cell::IO::HID* device;
    WiimoteCommunications::StatusReport currentStatus;
    WiimoteCommunications::ButtonsAccelExtensionReport currentInputData;
    WiimoteExtensionType extensionType;
};

}
