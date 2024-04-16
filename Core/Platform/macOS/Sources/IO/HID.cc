// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/IO/HID.hh>
#include <Cell/System/Panic.hh>

namespace Cell::IO::HID {

Wrapped<Device*, Result> Device::Open(const uint16_t vendorId, const uint16_t productId) {
    (void)(vendorId);
    (void)(productId);

    return Result::NotFound;
}

Device::~Device() {
    (void)(this->impl);

    CELL_UNIMPLEMENTED
}

Result Device::Read(Memory::IBlock& data, const uint32_t milliseconds) {
    (void)(data);
    (void)(milliseconds);

    CELL_UNIMPLEMENTED
}

Result Device::Write(const Memory::IBlock& data, const uint32_t milliseconds) {
    (void)(data);
    (void)(milliseconds);

    CELL_UNIMPLEMENTED
}

Wrapped<Capabilities, Result> Device::GetCapabilities() {
    CELL_UNIMPLEMENTED
}

}
