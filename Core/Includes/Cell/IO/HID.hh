// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Wrapped.hh>
#include <Cell/IO/Result.hh>
#include <Cell/Memory/Block.hh>

namespace Cell::IO::HID {

// Connection methods for HID devices.
enum class ConnectionType : uint8_t {
    USB,
    Bluetooth
};

// Defines the capabilities of a HID device's reports.
struct Capabilities {
    // The readable report size in bytes.
    uint16_t InputReportSize;

    // The writable report size in bytes.
    uint16_t OutputReportSize;

    // The size of the feature report.
    uint16_t FeatureReportSize;
};

// Allows for interacting with HID devices.
class Device : public NoCopyObject {
public:
    // Opens or creates a HID device. Cell supports both USB and Bluetooth based HID devices (e.g controllers).
    CELL_FUNCTION static Wrapped<Device*, Result> Open(const uint16_t vendorId, const uint16_t productId);

    // Closes the device.
    CELL_FUNCTION ~Device();

    // Reads data from the device. This would be a report for HID devices.
    // Allows terminating reading with a timeout.
    CELL_FUNCTION Result Read(Memory::IBlock& data, const uint32_t milliseconds = 0);

    // Writes data to the device. This would be a report for HID devices.
    // Allows terminating writing with a timeout.
    CELL_FUNCTION Result Write(const Memory::IBlock& data, const uint32_t milliseconds = 0);

    // Fetches the capabilities of the device.
    CELL_FUNCTION Wrapped<Capabilities, Result> GetCapabilities();

    // Returns the connection type this device uses.
    CELL_FUNCTION ConnectionType GetConnectionType() const;

private:
    CELL_FUNCTION_INTERNAL Device(uintptr_t i, const ConnectionType t) : impl(i), type(t) { }

    uintptr_t impl;
    const ConnectionType type;
};

}
