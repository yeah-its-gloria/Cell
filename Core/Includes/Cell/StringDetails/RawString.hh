// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::StringDetails {

// Returns the raw string size. This can only be used by constant expressed strings.
CELL_NODISCARD CELL_FUNCTION constexpr size_t RawStringSize(const char* string) {
    for (size_t i = 0; i < SIZE_MAX; i++) {
        if (string[i + 1] == '\0') {
            return i + 1;
        }
    }

    return 1;
}

}
