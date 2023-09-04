// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "Cell/System/Block.hh"
#include <Cell/Wrapped.hh>
#include <Cell/IO/Result.hh>

namespace Cell::IO {

// Allows for interacting with HID devices.
class USB : public Object {
public:
    // Opens or creates a USB device.
    CELL_FUNCTION static Wrapped<USB*, Result> Open(const uint16_t vendorId, const uint16_t productId, const uint8_t interface = 0);

    // Closes the device.
    CELL_FUNCTION ~USB();

    // Reads data from the device. This would be a report for HID devices.
    // Allows terminating reading with a timeout.
    CELL_FUNCTION Result Read(IBlock& data, const uint8_t endpoint, const uint32_t milliseconds = 0);

    // Writes data to the device. This would be a report for HID devices.
    // Allows terminating writing with a timeout.
    CELL_FUNCTION Result Write(const IBlock& data, const uint8_t endpoint, const uint32_t milliseconds = 0);

private:
    CELL_FUNCTION_INTERNAL USB(const uintptr_t handle) : handle(handle) { }

    uintptr_t handle;
};

}
