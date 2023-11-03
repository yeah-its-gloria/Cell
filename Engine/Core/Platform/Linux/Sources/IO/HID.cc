// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/IO/HID.hh>
#include <Cell/System/Panic.hh>

namespace Cell::IO::HID {

Device::~Device() {
    (void)(this->handle);

    CELL_UNIMPLEMENTED
}

Wrapped<Device*, Result> Device::Open(const uint16_t vendorId, const uint16_t productId) {
    (void)(vendorId);
    (void)(productId);

    CELL_UNIMPLEMENTED

    return new Device(0, ConnectionType::USB);
}

Result Device::Read(IBlock& data, const uint32_t milliseconds) {
    (void)(data);
    (void)(milliseconds);

    CELL_UNIMPLEMENTED
}

Result Device::Write(const IBlock& data, const uint32_t milliseconds) {
    (void)(data);
    (void)(milliseconds);

    CELL_UNIMPLEMENTED
}

Wrapped<Capabilities, Result> Device::GetCapabilities() {
    CELL_UNIMPLEMENTED
}

}
