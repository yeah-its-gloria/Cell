// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#ifdef CELL_MODULES_MATHEMATICS_HARDWARE_ACCELERATE_X86

#include <Cell/Mathematics/ExtendedIntegers.hh>

CELL_INLINE int128_t FusedMultiplyAdd(int128_t a, int128_t b, int128_t c) {
#if defined(__FMA__)
    return _mm_fmadd_ps(a, b, c);
#else
    return _mm_add_ps(c, _mm_mul_ps(a, b));
#endif
}

CELL_INLINE int128_t FusedNegativeMultiplyAdd(int128_t a, int128_t b, int128_t c) {
#if defined(__FMA__)
    return _mm_fnmadd_ps(a, b, c);
#else
    return _mm_sub_ps(c, _mm_mul_ps(a, b));
#endif
}

#endif

