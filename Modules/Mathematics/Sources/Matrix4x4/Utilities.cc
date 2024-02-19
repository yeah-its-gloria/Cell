// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Mathematics/Matrix4x4.hh>

#include <math.h>

#include <Cell/System/Panic.hh>

namespace Cell::Mathematics {

Matrix4x4 Matrix4x4::AsPerspective(const float fovY, const float aspect, const float nearClip, const float farClip) {
    const float fovTan = 1.f / tanf(fovY * 0.5f);
    const float depth = 1.f / (nearClip - farClip);

    return Matrix4x4((float[4][4]){ { fovTan / aspect,     0.f,                        0.f,  0.f },
                                    {             0.f, -fovTan,                        0.f,  0.f },
                                    {             0.f,     0.f,            farClip * depth, -1.f },
                                    {             0.f,     0.f, nearClip * farClip * depth,  0.f } });
}

}
