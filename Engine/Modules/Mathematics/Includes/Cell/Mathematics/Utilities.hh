// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#define CELL_MATHEMATICS_PI 3.14159265358979323846264338327950288f

namespace Cell::Mathematics::Utilities {

// Converts the given degrees value to radians.
CELL_INLINE constexpr float DegreesToRadians(const float degrees) {
    return degrees * CELL_MATHEMATICS_PI / 180.f;
}

}
