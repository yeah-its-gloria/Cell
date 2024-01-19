// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/IO/USB.hh>
#include <Cell/System/Panic.hh>

namespace Cell::IO {

Wrapped<USB*, Result> USB::Open(const uint16_t vendorId, const uint16_t productId, const uint8_t interface) {
    (void)(vendorId);
    (void)(productId);
    (void)(interface);

    // TODO: implement

    return Result::NotFound;
}

USB::~USB() {
    (void)(this->handle);

    // TODO: implement

    CELL_UNIMPLEMENTED
}

Result USB::Read(IBlock& data, const uint8_t endpoint, const uint32_t milliseconds) {
    (void)(data);
    (void)(endpoint);
    (void)(milliseconds);

    // TODO: implement

    CELL_UNIMPLEMENTED
}

Result USB::Write(const IBlock& data, const uint8_t endpoint, const uint32_t milliseconds) {
    (void)(data);
    (void)(endpoint);
    (void)(milliseconds);

    // TODO: implement

    CELL_UNIMPLEMENTED
}

}
