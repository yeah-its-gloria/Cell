// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Reference.hh>

namespace Cell { class String; }

namespace Cell::StringDetails::Formatting {

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
        const double floatingPoint;
        const Reference<const String> string;
    };
};

// Template function for implementing storage of various types.
// Also reminder that C++ reference rules make no sense
template <typename T> CELL_FUNCTION_TEMPLATE constexpr Data Package(T a);

#define IMPL_PRIMITIVE(T, TT, TTT) \
template <> CELL_FUNCTION_TEMPLATE constexpr Data Package<T>(T value) { return { .type = Type::TT, .TTT = value }; } \
template <> CELL_FUNCTION_TEMPLATE constexpr Data Package<const T>(const T value) { return { .type = Type::TT, .TTT = value }; } \
template <> CELL_FUNCTION_TEMPLATE constexpr Data Package<T&>(T& value) { return { .type = Type::TT, .TTT = value }; } \
template <> CELL_FUNCTION_TEMPLATE constexpr Data Package<const T&>(const T& value) { return { .type = Type::TT, .TTT = value }; }

#define IMPL_POINTER(T, TT, TTT) \
template <> CELL_FUNCTION_TEMPLATE constexpr Data Package<T*>(T* value) { return { .type = Type::TT, .TTT = value }; } \
template <> CELL_FUNCTION_TEMPLATE constexpr Data Package<const T*>(const T* value) { return { .type = Type::TT, .TTT = value }; } \
template <> CELL_FUNCTION_TEMPLATE constexpr Data Package<T*&>(T*& value) { return { .type = Type::TT, .TTT = value }; } \
template <> CELL_FUNCTION_TEMPLATE constexpr Data Package<const T*&>(const T*& value) { return { .type = Type::TT, .TTT = value }; }

IMPL_PRIMITIVE(uint8_t, UInt, uInt)
IMPL_PRIMITIVE(uint16_t, UInt, uInt)
IMPL_PRIMITIVE(uint32_t, UInt, uInt)
IMPL_PRIMITIVE(uint64_t, UInt, uInt)

IMPL_PRIMITIVE(int8_t, Int, sInt)
IMPL_PRIMITIVE(int16_t, Int, sInt)
IMPL_PRIMITIVE(int32_t, Int, sInt)
IMPL_PRIMITIVE(int64_t, Int, sInt)

IMPL_PRIMITIVE(float, FloatingPoint, floatingPoint)
IMPL_PRIMITIVE(double, FloatingPoint, floatingPoint)

IMPL_POINTER(char, ConstCharPointer, constCharPointer)
IMPL_POINTER(wchar_t, ConstWideCharPointer, constWideCharPointer)
IMPL_POINTER(void, Address, address)

template <> CELL_FUNCTION_TEMPLATE constexpr Data Package<String&>(String& value) { return { .type = Type::CellString, .string = value }; }
template <> CELL_FUNCTION_TEMPLATE constexpr Data Package<const String&>(const String& value) { return { .type = Type::CellString, .string = value }; }

#ifdef CELL_PLATFORM_WINDOWS
IMPL_PRIMITIVE(long, Int, sInt)
IMPL_PRIMITIVE(unsigned long, UInt, uInt)
#endif

#undef IMPL_POINTER
#undef IMPL_PRIMITIVE

}
