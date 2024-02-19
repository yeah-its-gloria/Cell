// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/DataManagement/Foreign/Base64.hh>

namespace Cell::DataManagement::Foreign {

Wrapped<uint8_t*, Result> Base64Decode(const String& data) {
    (void)(data);

    CELL_UNIMPLEMENTED
}


Wrapped<String, Result> Base64Encode(const uint8_t* CELL_NONNULL data) {
    (void)(data);

    CELL_UNIMPLEMENTED
}

}
