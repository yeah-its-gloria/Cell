// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Mathematics/Quaternion.hh>

#include <math.h>

namespace Cell::Mathematics {

Quaternion::Quaternion() : x(0), y(0), z(0), w(0) { }
Quaternion::Quaternion(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) { }
Quaternion::Quaternion(const float raw[4]) : x(raw[0]), y(raw[1]), z(raw[2]), w(raw[3]) { }
Quaternion::Quaternion(const float value) : x(value), y(value), z(value), w(value) { }

Quaternion Quaternion::FromRotation(const float angle, const Vector3& axis) {
    const float sine = sinf(angle);
    const Vector3 normAxis = axis.Normalize();

    return Quaternion(sine * normAxis.x, sine * normAxis.y, sine * normAxis.z, cosf(angle));
}

}
