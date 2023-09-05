// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::Shell {

// Various key values for keyboards.
enum class KeyValue : uint64_t {
    None = 0ull,
    Zero = 1ull << 0,
    One = 1ull << 1,
    Two = 1ull << 2,
    Three = 1ull << 3,
    Four = 1ull << 4,
    Five = 1ull << 5,
    Six = 1ull << 6,
    Seven = 1ull << 7,
    Eight = 1ull << 8,
    Nine = 1ull << 9,
    A = 1ull << 10,
    B = 1ull << 11,
    C = 1ull << 12,
    D = 1ull << 13,
    E = 1ull << 14,
    F = 1ull << 15,
    G = 1ull << 16,
    H = 1ull << 17,
    I = 1ull << 18,
    J = 1ull << 19,
    K = 1ull << 20,
    L = 1ull << 21,
    M = 1ull << 22,
    N = 1ull << 23,
    O = 1ull << 24,
    P = 1ull << 25,
    Q = 1ull << 26,
    R = 1ull << 27,
    S = 1ull << 28,
    T = 1ull << 29,
    U = 1ull << 30,
    V = 1ull << 31,
    W = 1ull << 32,
    X = 1ull << 33,
    Y = 1ull << 34,
    Z = 1ull << 35
};

CELL_INLINE KeyValue operator|(KeyValue a, KeyValue b) {
    return (KeyValue)((uint64_t)a | (uint64_t)b);
}

CELL_INLINE KeyValue& operator|=(KeyValue& a, KeyValue b) {
    a = a | b;
    return a;
}

CELL_INLINE KeyValue operator^(KeyValue a, KeyValue b) {
    return (KeyValue)((uint64_t)a ^ (uint64_t)b);
}

CELL_INLINE KeyValue& operator^=(KeyValue& a, KeyValue b) {
    a = a ^ b;
    return a;
}

CELL_INLINE KeyValue operator&(KeyValue a, KeyValue b) {
    return (KeyValue)((uint64_t)a & (uint64_t)b);
}

CELL_INLINE KeyValue& operator&=(KeyValue& a, KeyValue b) {
    a = a & b;
    return a;
}

}
