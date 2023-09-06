// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/IO/HID.hh>
#include <Cell/System/Panic.hh>

namespace Cell::IO {

HID::~HID() {
    (void)(this->handle);

    CELL_UNIMPLEMENTED
}

Wrapped<HID*, Result> HID::Open(const uint16_t vendorId, const uint16_t productId) {
    (void)(vendorId);
    (void)(productId);

    CELL_UNIMPLEMENTED

    return new HID(0);
}

Result HID::Read(IBlock& data, const uint32_t milliseconds) {
    (void)(data);
    (void)(milliseconds);

    CELL_UNIMPLEMENTED
}

Result HID::Write(const IBlock& data, const uint32_t milliseconds) {
    (void)(data);
    (void)(milliseconds);

    CELL_UNIMPLEMENTED
}

Wrapped<HIDCapabilities, Result> HID::GetCapabilities() {
    CELL_UNIMPLEMENTED
}

}
