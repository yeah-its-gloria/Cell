// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

#define CELL_BASE_TYPE(a) __underlying_type(a)

// Defines commonly used enumerator operators for enumerator classes.
#define CELL_ENUM_CLASS_OPERATORS(N) \
    CELL_INLINE N operator|(N a, N b) { return (N)((CELL_BASE_TYPE(N))a | (CELL_BASE_TYPE(N))b); } \
    CELL_INLINE N& operator|=(N& a, N b) { a = a | b; return a; } \
    CELL_INLINE N operator^(N a, N b) { return (N)((CELL_BASE_TYPE(N))a ^ (CELL_BASE_TYPE(N))b); } \
    CELL_INLINE N& operator^=(N& a, N b) { a = a ^ b; return a; } \
    CELL_INLINE N operator&(N a, N b) { return (N)((CELL_BASE_TYPE(N))a & (CELL_BASE_TYPE(N))b); } \
    CELL_INLINE N& operator&=(N& a, N b) { a = a & b; return a; }

// Packing attribute for offsets in structures.
#define CELL_PACKED(x) __attribute__((packed, aligned(x)))
