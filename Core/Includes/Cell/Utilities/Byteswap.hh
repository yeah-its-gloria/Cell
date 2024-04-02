// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::Utilities {

// Swaps the bytes of an unsigned 16 bit integer.
CELL_NODISCARD CELL_FUNCTION_TEMPLATE uint16_t Byteswap(const uint16_t a) {
    return a >> 8 | a << 8;
}

// Swaps the bytes of an unsigned 32 bit integer.
CELL_NODISCARD CELL_FUNCTION_TEMPLATE uint32_t Byteswap(const uint32_t a) {
    return ((a >> 24) & 0x000000ff) |
           ((a >>  8) & 0x0000ff00) |
           ((a <<  8) & 0x00ff0000) |
           ((a << 24) & 0xff000000);
}

// Swaps the bytes of an unsigned 64 bit integer.
CELL_NODISCARD CELL_FUNCTION_TEMPLATE uint64_t Byteswap(const uint64_t a) {
    return ((a >> 56) & 0x00000000000000ff) |
           ((a >> 40) & 0x000000000000ff00) |
           ((a >> 24) & 0x0000000000ff0000) |
           ((a >>  8) & 0x00000000ff000000) |
           ((a <<  8) & 0x000000ff00000000) |
           ((a << 24) & 0x0000ff0000000000) |
           ((a << 40) & 0x00ff000000000000) |
           ((a << 56) & 0xff00000000000000);
}

// Performs a byteswap on little endian encoded data if the platform is big endian.
CELL_NODISCARD CELL_FUNCTION_TEMPLATE uint16_t ByteswapFromLE(const uint16_t a) {
#ifdef CELL_PLATFORM_IS_BIG_ENDIAN
    return Byteswap(a);
#else
    return a;
#endif
}
// Performs a byteswap on little endian encoded data if the platform is big endian.
CELL_NODISCARD CELL_FUNCTION_TEMPLATE uint32_t ByteswapFromLE(const uint32_t a) {
#ifdef CELL_PLATFORM_IS_BIG_ENDIAN
    return Byteswap(a);
#else
    return a;
#endif
}

// Performs a byteswap on little endian encoded data if the platform is big endian.
CELL_NODISCARD CELL_FUNCTION_TEMPLATE uint64_t ByteswapFromLE(const uint64_t a) {
#ifdef CELL_PLATFORM_IS_BIG_ENDIAN
    return Byteswap(a);
#else
    return a;
#endif
}

// Performs a byteswap on big/network endian encoded data if the platform is little endian.
CELL_NODISCARD CELL_FUNCTION_TEMPLATE uint16_t ByteswapFromBE(const uint16_t a) {
#ifdef CELL_PLATFORM_IS_BIG_ENDIAN
    return a;
#else
    return Byteswap(a);
#endif
}

// Performs a byteswap on big/network endian encoded data if the platform is little endian.
CELL_NODISCARD CELL_FUNCTION_TEMPLATE uint32_t ByteswapFromBE(const uint32_t a) {
#ifdef CELL_PLATFORM_IS_BIG_ENDIAN
    return a;
#else
    return Byteswap(a);
#endif
}

// Performs a byteswap on big/network endian encoded data if the platform is little endian.
CELL_NODISCARD CELL_FUNCTION_TEMPLATE uint64_t ByteswapFromBE(const uint64_t a) {
#ifdef CELL_PLATFORM_IS_BIG_ENDIAN
    return a;
#else
    return Byteswap(a);
#endif
}

}
