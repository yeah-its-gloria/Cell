// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

#define CELL_BASE_TYPE(t) __underlying_type(t)
#define CELL_IS_STANDARD_LAYOUT(t) __is_standard_layout(t)

// Defines commonly used enumerator operators for enumerator classes.
#define CELL_ENUM_CLASS_OPERATORS(N) \
    CELL_FUNCTION_TEMPLATE constexpr N operator|(const N a, const N b) { return (N)((CELL_BASE_TYPE(N))a | (CELL_BASE_TYPE(N))b); } \
    CELL_FUNCTION_TEMPLATE constexpr N& operator|=(N& a, const N b) { a = a | b; return a; } \
    CELL_FUNCTION_TEMPLATE constexpr N operator^(const N a, const N b) { return (N)((CELL_BASE_TYPE(N))a ^ (CELL_BASE_TYPE(N))b); } \
    CELL_FUNCTION_TEMPLATE constexpr N& operator^=(N& a, const N b) { a = a ^ b; return a; } \
    CELL_FUNCTION_TEMPLATE constexpr N operator&(const N a, const N b) { return (N)((CELL_BASE_TYPE(N))a & (CELL_BASE_TYPE(N))b); } \
    CELL_FUNCTION_TEMPLATE constexpr N& operator&=(N& a, const N b) { a = a & b; return a; }

// Packing attribute for offsets in structures.
#define CELL_PACKED(x) __attribute__((packed, aligned(x)))

#ifdef CELL_PLATFORM_WINDOWS
#define CELL_COUNT_OF(x) _countof(x)
#else
#define CELL_COUNT_OF(x) (sizeof(x) / sizeof(*x))
#endif
