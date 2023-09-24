// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Wrapped.hh>
#include <Cell/IO/Result.hh>
#include <Cell/System/Block.hh>

namespace Cell::IO {

// Defines the capabilities of a HID device's reports.
struct HIDCapabilities {
    // The readable report size in bytes.
    uint16_t InputReportSize;

    // The writable report size in bytes.
    uint16_t OutputReportSize;

    // The size of the feature report.
    uint16_t FeatureReportSize;
};

// Allows for interacting with HID devices.
class HID : public Object {
public:
    // Opens or creates a HID device. Cell supports both USB and Bluetooth based HID devices (e.g controllers).
    CELL_FUNCTION static Wrapped<HID*, Result> Open(const uint16_t vendorId, const uint16_t productId);

    // Closes the device.
    CELL_FUNCTION ~HID();

    // Reads data from the device. This would be a report for HID devices.
    // Allows terminating reading with a timeout.
    CELL_FUNCTION Result Read(IBlock& data, const uint32_t milliseconds = 0);

    // Writes data to the device. This would be a report for HID devices.
    // Allows terminating writing with a timeout.
    CELL_FUNCTION Result Write(const IBlock& data, const uint32_t milliseconds = 0);

    // Fetches the capabilities of the device.
    CELL_FUNCTION Wrapped<HIDCapabilities, Result> GetCapabilities();

private:
    CELL_FUNCTION_INTERNAL HID(const uintptr_t handle) : handle(handle) { }

    uintptr_t handle;
};

}
