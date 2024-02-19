// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Mathematics/Vector2.hh>

namespace Cell::Mathematics {

Vector2::Vector2() : x(0), y(0) { }
Vector2::Vector2(const float x, const float y) : x(x), y(y) { }
Vector2::Vector2(const float value) : x(value), y(value) { }

}
