// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::Utilities {

// Swaps an unsigned, 16 bit integer.
CELL_NODISCARD CELL_INLINE uint16_t Byteswap(const uint16_t a) {
    return a >> 8 | a << 8;
}

// Swaps an unsigned, 32 bit integer.
CELL_NODISCARD CELL_INLINE uint32_t Byteswap(const uint32_t a) {
    return ((a >> 24) & 0x000000ff) |
           ((a >> 8) & 0x0000ff00) |
           ((a << 8) & 0x00ff0000) |
           ((a << 24) & 0xff000000);
}

// Swaps an unsigned, 64 bit integer.
CELL_NODISCARD CELL_INLINE uint64_t Byteswap(const uint64_t a) {
    return ((a >> 56) & 0x00000000000000ff) |
           ((a >> 40) & 0x000000000000ff00) |
           ((a >> 24) & 0x0000000000ff0000) |
           ((a >> 8) & 0x00000000ff000000) |
           ((a << 8) & 0x000000ff00000000) |
           ((a << 24) & 0x0000ff0000000000) |
           ((a << 40) & 0x00ff000000000000) |
           ((a << 56) & 0xff00000000000000);
}

// Swaps a little endian value to fit the platform endianness and vice versa, if needed.
CELL_NODISCARD CELL_INLINE uint16_t ByteswapLEPlatform(const uint16_t a) {
#ifdef CELL_PLATFORM_IS_BIG_ENDIAN
    return Byteswap(a);
#else
    return a;
#endif
}

// Swaps a little endian value to fit the platform endianness and vice versa, if needed.
CELL_NODISCARD CELL_INLINE uint32_t ByteswapLEPlatform(const uint32_t a) {
#ifdef CELL_PLATFORM_IS_BIG_ENDIAN
    return Byteswap(a);
#else
    return a;
#endif
}

// Swaps a little endian value to fit the platform endianness and vice versa, if needed.
CELL_NODISCARD CELL_INLINE uint64_t ByteswapLEPlatform(const uint64_t a) {
#ifdef CELL_PLATFORM_IS_BIG_ENDIAN
    return Byteswap(a);
#else
    return a;
#endif
}

// Swaps a big/network endian value to fit the platform endianness and vice versa, if needed.
CELL_NODISCARD CELL_INLINE uint16_t ByteswapBEPlatform(const uint16_t a) {
#ifdef CELL_PLATFORM_IS_BIG_ENDIAN
    return a;
#else
    return Byteswap(a);
#endif
}

// Swaps a big/network endian value to fit the platform endianness and vice versa, if needed.
CELL_NODISCARD CELL_INLINE uint32_t ByteswapBEPlatform(const uint32_t a) {
#ifdef CELL_PLATFORM_IS_BIG_ENDIAN
    return a;
#else
    return Byteswap(a);
#endif
}

// Swaps a big/network endian value to fit the platform endianness and vice versa, if needed.
CELL_NODISCARD CELL_INLINE uint64_t ByteswapBEPlatform(const uint64_t a) {
#ifdef CELL_PLATFORM_IS_BIG_ENDIAN
    return a;
#else
    return Byteswap(a);
#endif
}

}
