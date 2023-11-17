// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/System/Panic.hh>

namespace Cell {

// Possibly empty value.
// Lifetime of the contained value is the responsibility of callers.
template <typename T> class Optional : public Object {
public:
    // Creates a filled optional value.
    CELL_INLINE Optional(T data) : data(data), hasData(true) { }

    // Creates an empty optional value.
    CELL_INLINE Optional() : hasData(false) { }

    // Empty destructor.
    CELL_INLINE ~Optional() { }

    // Returns the wrapped value.
    CELL_NODISCARD CELL_INLINE T Unwrap() const {
        CELL_ASSERT(this->hasData);
        return this->data;
    }

    // Checks if this optional value contains data.
    CELL_NODISCARD CELL_INLINE bool IsValid() const {
        return this->hasData;
    }

    // Allows assigning new data to fill this optional value.
    CELL_INLINE Optional<T>& operator =(const T& data) {
        this->data = data;
        this->hasData = true;

        return *this;
    }

    // Operator shortcut for IsValid.
    CELL_INLINE operator bool() const {
        return this->hasData;
    }

private:
    struct Dummy { constexpr Dummy() { }; };
    CELL_STATIC_ASSERT(!__is_trivially_constructible(Dummy));

    union {
        Dummy _dummy;
        T data;
    };

    bool hasData;
};

template <typename T> Optional<T> None() {
    return Optional<T>();
}

}
