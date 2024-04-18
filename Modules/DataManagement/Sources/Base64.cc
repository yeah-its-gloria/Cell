// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/DataManagement/Base64.hh>

namespace Cell::DataManagement {

Wrapped<uint8_t*, Result> Base64Decode(const String& data) {
    (void)(data);

    CELL_UNIMPLEMENTED
}

Wrapped<String, Result> Base64Encode(const Memory::IBlock& data) {
    (void)(data);

    CELL_UNIMPLEMENTED
}

}
