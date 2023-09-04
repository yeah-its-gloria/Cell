// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/DataManagement/Foreign/HTTP.hh>
#include <Cell/System/Memory.hh>
#include <Cell/System/Panic.hh>

#include <string.h>

namespace Cell::DataManagement::Foreign {

Result HTTPParseResponse(const uint8_t* data, const size_t size) {
    if (size == 0) {
        return Result::InvalidParameters;
    }

    const char* httpIdentifier = "HTTP/";
    const bool result = System::CompareMemory<uint8_t>(data, (uint8_t*)httpIdentifier, strlen(httpIdentifier));
    CELL_ASSERT(result);

    return Result::Success;
}

}
