// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#define CELL_MATHEMATICS_PI 3.14159265358979323846264338327950288

namespace Cell::Mathematics::Utilities {

// Converts the given degrees value to radians.
CELL_INLINE constexpr double DegreesToRadians(const double degrees) {
    return degrees * CELL_MATHEMATICS_PI / 180.f;
}

}
