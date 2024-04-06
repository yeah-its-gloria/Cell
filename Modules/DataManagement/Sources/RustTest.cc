// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/DataManagement/RustTest.hh>

#include <CellDataManagementRustImplementation.hh>

namespace Cell::DataManagement {

uint8_t RustTest() {
    Rust::CAPI_Test* test = Rust::TestNew();
    const uint8_t value = Rust::TestGetValue(test);
    Rust::TestDestruct(test);
    return value;
}

}
