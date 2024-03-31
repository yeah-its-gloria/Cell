// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::Utilities {

// Clamps the desired value to at least the given minimum value.
template <typename T> CELL_NODISCARD CELL_FUNCTION_TEMPLATE constexpr T Minimum(const T value, const T minimum) {
    return value < minimum ? minimum : value;
}

// Clamps the desired value to no more than the given maximum value.
template <typename T> CELL_NODISCARD CELL_FUNCTION_TEMPLATE constexpr T Maximum(const T value, const T maximum) {
    return value > maximum ? maximum : value;
}

// Clamps the desired value between the given minimum and maximum.
template <typename T> CELL_NODISCARD CELL_FUNCTION_TEMPLATE constexpr T Clamp(const T value, const T minimum, const T maximum) {
    return Minimum(Maximum(value, maximum), minimum);
}

}
