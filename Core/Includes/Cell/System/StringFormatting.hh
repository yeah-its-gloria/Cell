// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::System { class String; }

namespace Cell::System::StringFormatting {

// Describes possible output types for formatting.
enum class Type {
    // Bare signed integer.
    Int,

    // Bare unsigned integer.
    UInt,

    // Regular C const char pointer string.
    ConstCharPointer,

    // Platform specific wide character pointer string.
    ConstWideCharPointer,

    // Address. Formatted hexadecimally.
    Address,

    // Represents the string class.
    CellString,

    FloatingPoint
};

// Structure holding data relevant formatting.
struct Data {
    Type type;
    union {
        const signed long long sInt;
        const unsigned long long uInt;
        const char* constCharPointer;
        const wchar_t* constWideCharPointer;
        const void* address;
        const System::String& string;
        const double floatingPoint;
    };
};

// Template function for implementing storage of various types.
// Also reminder that C++ reference rules make no sense
template <typename T> CELL_INLINE constexpr Data Package(T a);

#define IMPL_PRIMITIVE(T, TT, TTT) \
template <> CELL_INLINE constexpr Data Package<T&>(T& value) { return { .type = Type::TT, .TTT = value }; } \
template <> CELL_INLINE constexpr Data Package<const T&>(const T& value) { return { .type = Type::TT, .TTT = value }; }

#define IMPL_BRAIN_HURT(T, TT, TTT) \
template <> CELL_INLINE constexpr Data Package<T*>(T* value) { return { .type = Type::TT, .TTT = value }; } \
template <> CELL_INLINE constexpr Data Package<const T*>(const T* value) { return { .type = Type::TT, .TTT = value }; } \
template <> CELL_INLINE constexpr Data Package<T*&>(T*& value) { return { .type = Type::TT, .TTT = value }; } \
template <> CELL_INLINE constexpr Data Package<const T*&>(const T*& value) { return { .type = Type::TT, .TTT = value }; }

IMPL_PRIMITIVE(uint8_t, UInt, uInt)
IMPL_PRIMITIVE(uint16_t, UInt, uInt)
IMPL_PRIMITIVE(uint32_t, UInt, uInt)
IMPL_PRIMITIVE(uint64_t, UInt, uInt)

IMPL_PRIMITIVE(int8_t, Int, sInt)
IMPL_PRIMITIVE(int16_t, Int, sInt)
IMPL_PRIMITIVE(int32_t, Int, sInt)
IMPL_PRIMITIVE(int64_t, Int, sInt)

IMPL_BRAIN_HURT(char, ConstCharPointer, constCharPointer)
IMPL_BRAIN_HURT(wchar_t, ConstWideCharPointer, constWideCharPointer)

template <> CELL_INLINE constexpr Data Package<float&>(float& value) { return { .type = Type::FloatingPoint, .floatingPoint = value }; }
template <> CELL_INLINE constexpr Data Package<double&>(double& value) { return { .type = Type::FloatingPoint, .floatingPoint = value }; }

template <> CELL_INLINE constexpr Data Package<void*&>(void*& value) { return { .type = Type::Address, .address = value }; }

template <> CELL_INLINE constexpr Data Package<String&>(String& value) { return { .type = Type::CellString, .string = value }; }

}
