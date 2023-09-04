// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/IO/USB.hh>
#include <Cell/System/Panic.hh>

namespace Cell::IO {

USB::~USB() {
    CELL_UNIMPLEMENTED
}

Wrapped<USB*, Result> USB::Open(const uint16_t vendorId, const uint16_t productId) {
    (void)(vendorId);
    (void)(productId);

    CELL_UNIMPLEMENTED

    return new USB(0, false);
}

Result USB::Read(IBlock& data, const uint32_t milliseconds) {
    (void)(data);
    (void)(milliseconds);

    (void)(this->handle);
    (void)(this->isHid);

    CELL_UNIMPLEMENTED
}

Result USB::Write(const IBlock& data, const uint32_t milliseconds) {
    (void)(data);
    (void)(milliseconds);

    CELL_UNIMPLEMENTED
}

}
