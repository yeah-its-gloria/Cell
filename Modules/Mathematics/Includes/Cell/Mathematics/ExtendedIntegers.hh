// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

#ifdef CELL_MODULES_MATHEMATICS_HARDWARE_ACCELERATE_X86
#include <x86intrin.h>

typedef __m128 int128_t;
typedef __m128_u uint128_t;

typedef __m256 int256_t;
typedef __m256_u uint256_t;
#else
#error Using hardware accelerated mathematics without any definition for the current platform.
#endif
