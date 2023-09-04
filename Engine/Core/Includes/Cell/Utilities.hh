// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::Utilities {

// Picks the smaller value of the two.
template <typename T>
CELL_NODISCARD CELL_INLINE T Minimum(const T value, const T minimum) {
    return value < minimum ? minimum : value;
}

// Picks the larger value of the two.
template <typename T>
CELL_NODISCARD CELL_INLINE T Maximum(const T value, const T maximum) {
    return value > maximum ? maximum : value;
}

// Clamps value between minimum and maximum.
template <typename T>
CELL_NODISCARD CELL_INLINE T Clamp(const T value, const T minimum, const T maximum) {
    return Minimum(Maximum(value, maximum), minimum);
}

// Returns the raw string size.
CELL_NODISCARD CELL_INLINE constexpr size_t RawStringSize(const char* string) {
    for (size_t i = 0; i < SIZE_MAX; i++) {
        if (string[i + 1] == '\0') {
            return i + 1;
        }
    }

    return 1;
}

}
