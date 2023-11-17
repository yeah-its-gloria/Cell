// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/IO/USB.hh>
#include <Cell/System/Panic.hh>

namespace Cell::IO {

USB::~USB() {
    (void)(this->handle);

    CELL_UNIMPLEMENTED
}

Wrapped<USB*, Result> USB::Open(const uint16_t vendorId, const uint16_t productId, const uint8_t interface) {
    (void)(vendorId);
    (void)(productId);
    (void)(interface);

    CELL_UNIMPLEMENTED

    return new USB(0);
}

Result USB::Read(IBlock& data, const uint8_t endpoint, const uint32_t milliseconds) {
    (void)(data);
    (void)(endpoint);
    (void)(milliseconds);

    CELL_UNIMPLEMENTED
}

Result USB::Write(const IBlock& data, const uint8_t endpoint, const uint32_t milliseconds) {
    (void)(data);
    (void)(endpoint);
    (void)(milliseconds);

    CELL_UNIMPLEMENTED
}

}
