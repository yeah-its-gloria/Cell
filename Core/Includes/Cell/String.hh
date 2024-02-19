// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Wrapped.hh>
#include <Cell/StringDetails/Formatting.hh>
#include <Cell/StringDetails/Result.hh>
#include <Cell/StringDetails/RawString.hh>

namespace Cell {

// Represents a block of text, encoded as UTF-8.
class String : public Object {
public:
    // Creates an empty string.
    CELL_FUNCTION String();

    // Creates a string from UTF-8 encoded data. Leaving length 0 makes it check for null termination instead.
    CELL_FUNCTION String(const char* CELL_NONNULL utf8, const size_t length = 0);

    // Copies another string.
    CELL_FUNCTION String(const String& string);

    // Destructs the string.
    CELL_FUNCTION ~String();

    // Creates a string from a wide string, which converts its contents from the platform encoding to UTF-8 automatically.
    CELL_NODISCARD CELL_FUNCTION static Wrapped<String, StringDetails::Result> FromPlatformWideString(const wchar_t* CELL_NONNULL wide);

    // Formats the given content into the string.
    // Use "%" to indicate where to insert an object.
    // TODO: formatting options, like indicating location and how to print specific numbers
    template <typename... T> CELL_NODISCARD CELL_INLINE static String Format(const char* format, T&&... args) {
        return FormatImplementation(format,
                                    StringDetails::RawStringSize(format),
                                    (const StringDetails::Formatting::Data[sizeof...(T)]){ StringDetails::Formatting::Package<T>(args)... },
                                    sizeof...(T));
    }

    // Appends another string.
    CELL_FUNCTION StringDetails::Result Append(const String& string);

    // Appends UTF-8 encoded text data.
    CELL_FUNCTION StringDetails::Result Append(const char* CELL_NONNULL data);

    // Replaces the contents of the string with new, raw contents. By default, it clears it.
    CELL_FUNCTION void Clear();

    // Returns the number of bytes inside the string. 0 equals empty.
    CELL_NODISCARD CELL_FUNCTION size_t GetSize() const;

    // Returns the number of characters in this string.
    CELL_NODISCARD CELL_FUNCTION size_t GetCount() const;

    // Utility function to check whether the string is currently empty.
    CELL_NODISCARD CELL_FUNCTION bool IsEmpty() const;

    // Checks if this string begins with the given substring.
    CELL_NODISCARD CELL_FUNCTION bool BeginsWith(const String& substring) const;

    // Checks if this string ends with the given
    CELL_NODISCARD CELL_FUNCTION bool EndsWith(const String& substring) const;

    // Cuts the string to the given offset and length, from its beginning.
    CELL_NODISCARD CELL_FUNCTION Wrapped<String, StringDetails::Result> Substring(const size_t offset, const size_t length) const;

    // Returns a null terminated C char buffer, UTF-8 encoded.
    // The buffer lifetime is caller managed.
    CELL_NODISCARD CELL_FUNCTION char* CELL_NONNULL ToCharPointer() const;

    // Returns a raw pointer to the underlying data.
    // This pointer must not be freed.
    CELL_NODISCARD CELL_FUNCTION const char* CELL_NONNULL ToRawPointer() const;

    // Returns a null terminated wide string buffer, generally UTF-16 encoded.
    // The buffer lifetime is caller managed.
    CELL_NODISCARD CELL_FUNCTION wchar_t* CELL_NONNULL ToPlatformWideString() const;

    // Converts the string in its entirety to a number.
    CELL_NODISCARD CELL_FUNCTION Wrapped<uint64_t, StringDetails::Result> AsNumber(const bool isHex = false) const;

    // Setting operator overwriting the string contents.
    CELL_FUNCTION String& operator = (const String& input);

    // CELL_FUNCTION operator overwriting the string contents.
    CELL_FUNCTION String& operator = (const char* input);

    // Comparison operator.
    CELL_NODISCARD CELL_FUNCTION bool operator == (const String& other) const;

    // Comparison operator.
    CELL_NODISCARD CELL_FUNCTION bool operator != (const String& other) const;

    // Appending operator.
    CELL_NODISCARD CELL_FUNCTION String operator + (const String& input) const;

    // Appending operator for UTF-8 encoded data.
    CELL_NODISCARD CELL_FUNCTION String operator + (const char* input) const;

    // Appending operator.
    CELL_FUNCTION void operator += (const String& input);

    // Appending operator for UTF-8 encoded data.
    CELL_FUNCTION void operator += (const char* input);

    // Per character iterator.
    CELL_NODISCARD CELL_FUNCTION char* begin();

    // Per character iterator.
    CELL_NODISCARD CELL_FUNCTION char* end();

private:
    CELL_FUNCTION static String FormatImplementation(const char* format, const size_t length, const StringDetails::Formatting::Data* content, const size_t count);

    char* data;
    size_t size;
};

}
