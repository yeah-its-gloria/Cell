// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Wrapped.hh>
#include <Cell/System/Result.hh>

namespace Cell::System {

// Represents a block of text, encoded as UTF-8.
class String : public Object {
public:
    // Creates an empty string.
    CELL_INLINE String() : data(nullptr), size(0) { }

    // Creates a string from UTF-8 encoded data. Leaving length 0 makes it check for null termination instead.
    CELL_FUNCTION String(const char* CELL_NONNULL utf8, size_t length = 0);

    // Copies another string.
    CELL_FUNCTION String(const String& string);

    // Creates a string from a wide string, which converts its contents from the platform encoding to UTF-8 automatically.
    CELL_FUNCTION static Wrapped<String, Result> FromPlatformWideString(const wchar_t* CELL_NONNULL wide);

    // Creates a string from a format string and arguments.
    CELL_FUNCTION __attribute__((format(printf, 1, 2))) static String Format(const char* CELL_NONNULL format, ...);

    // Destructs the string.
    CELL_FUNCTION ~String();

    // Appends another string.
    CELL_FUNCTION Result Append(const String& string);

    // Appends UTF-8 encoded text data.
    CELL_FUNCTION Result Append(const char* CELL_NONNULL data);

    // Replaces the contents of the string. By default, it empties it.
    CELL_FUNCTION void Set(const char* CELL_NONNULL data = "");

    // Returns the number of bytes inside the string. 0 equals empty.
    CELL_NODISCARD CELL_INLINE size_t GetSize() const { return this->size; }

    // Returns a null terminated C char buffer, UTF-8 encoded. The buffer lifetime is caller managed.
    CELL_NODISCARD CELL_FUNCTION char* CELL_NONNULL ToCharPointer() const;

    // Returns a raw pointer to the underlying data. This pointer must not be freed.
    CELL_NODISCARD CELL_INLINE const char* CELL_NONNULL ToRawPointer() const { return this->data; }

    // Returns a null terminated wide string buffer, generally UTF-16 encoded. The buffer lifetime is caller managed.
    CELL_NODISCARD CELL_FUNCTION wchar_t* CELL_NONNULL ToPlatformWideString() const;

    // Utility function to check whether the string is currently empty.
    CELL_NODISCARD CELL_INLINE bool IsEmpty() const { return this->GetSize() == 0; };

    // Checks if this string begins with the given substring.
    CELL_NODISCARD CELL_FUNCTION bool BeginsWith(const String& substring) const;

    // Checks if this string ends with the given
    CELL_NODISCARD CELL_FUNCTION bool EndsWith(const String& substring) const;

    // Cuts the string to the given offset and length, from its beginning.
    CELL_NODISCARD CELL_FUNCTION Wrapped<String, Result> Substring(const size_t offset, const size_t length) const;

    // Converts the string in its entirety to a number.
    CELL_NODISCARD CELL_FUNCTION Wrapped<uint64_t, Result> AsNumber(const bool isHex = false) const;

    // Returns the number of characters in this string.
    CELL_NODISCARD CELL_FUNCTION size_t GetCount() const;

    // Comparison operator.
    CELL_FUNCTION bool operator ==(const String& other);

    // Comparison operator.
    CELL_INLINE bool operator !=(const String& other) { return !(*this == other); }

    // Appending operator.
    CELL_INLINE String operator +(const String& input) { String str(this->data, this->size); str.Append(input); return str; }

    // Appending operator for UTF-8 encoded data.
    CELL_INLINE String operator +(const char* input) { String str(this->data, this->size); str.Append(input); return str; }

    // Appending operator.
    CELL_INLINE void operator +=(const String& input) { this->Append(input); }

    // Appending operator for UTF-8 encoded data.
    CELL_INLINE void operator +=(const char* input) { this->Append(input); }

    // Setting operator overwriting the string contents.
    CELL_FUNCTION String& operator =(const String& input);

    // Setting operator overwriting the string contents.
    CELL_INLINE String& operator =(const char* input) {
        this->Set(input);
        return *this;
    }

private:
    char* data;
    size_t size;
};

}
