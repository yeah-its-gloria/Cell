// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Mathematics/Vector3.hh>

namespace Cell::Mathematics {

Vector3::Vector3() : x(0), y(0), z(0) { }
Vector3::Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) { }
Vector3::Vector3(const float value) : x(value), y(value), z(value) { }

}
