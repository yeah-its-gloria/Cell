// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/IO/HID.hh>
#include <Cell/System/Panic.hh>

namespace Cell::IO::HID {

Wrapped<Device*, Result> Device::Open(const uint16_t vendorId, const uint16_t productId) {
    (void)(vendorId);
    (void)(productId);

    // TODO: implement

    return Result::NotFound;
}

Device::~Device() {
    (void)(this->handle);

    // TODO: implement

    CELL_UNIMPLEMENTED
}

Result Device::Read(IBlock& data, const uint32_t milliseconds) {
    (void)(data);
    (void)(milliseconds);

    // TODO: implement

    CELL_UNIMPLEMENTED
}

Result Device::Write(const IBlock& data, const uint32_t milliseconds) {
    (void)(data);
    (void)(milliseconds);

    // TODO: implement

    CELL_UNIMPLEMENTED
}

Wrapped<Capabilities, Result> Device::GetCapabilities() {
    // TODO: implement

    CELL_UNIMPLEMENTED
}

}
