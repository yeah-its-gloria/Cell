// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Wrapped.hh>
#include <Cell/IO/Result.hh>
#include <Cell/System/Block.hh>

namespace Cell::IO {

// Interactions with generic USB devices.
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

    CELL_NON_COPYABLE(USB)

private:
    CELL_HANDLE_CONSTRUCTOR(USB)

    const uintptr_t handle;
};

}
