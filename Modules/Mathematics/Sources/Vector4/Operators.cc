// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Mathematics/Vector4.hh>

#ifdef CELL_MODULES_MATHEMATICS_HARDWARE_ACCELERATE_X86

#include <Cell/Mathematics/ExtendedIntegers.hh>

#endif

namespace Cell::Mathematics {

Vector4 Vector4::operator *(const float scale) const {
#ifdef CELL_MODULES_MATHEMATICS_HARDWARE_ACCELERATE_X86
    float data1[4] = { this->x, this->y, this->z, this->w };
    float out[4] = { 0.f, 0.f, 0.f, 0.f };

    _mm_store_ps(out, _mm_mul_ps(_mm_load_ps(data1), _mm_set1_ps(scale)));

    return { out[0], out[1], out[2], out[3] };
#else
    return { this->x * scale, this->y * scale, this->z * scale, this->z * scale };
#endif
}

}
