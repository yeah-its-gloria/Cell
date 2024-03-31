// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/System/Panic.hh>

namespace Cell {

// Possibly empty value.
// Lifetime of the contained value is the responsibility of callers.
template <typename T> class Optional : public Object {
public:
    // Creates a filled optional value.
    CELL_FUNCTION_INTERNAL constexpr Optional(T data) : data(data), hasData(true) { }

    // Creates an empty optional value.
    CELL_FUNCTION_INTERNAL constexpr Optional() : hasData(false) { }

    // Trivial empty destructor.
    CELL_FUNCTION_INTERNAL constexpr ~Optional() { }

    // Returns the wrapped value.
    CELL_NODISCARD CELL_FUNCTION_INTERNAL T Unwrap() {
        CELL_ASSERT(this->hasData);

        return this->data;
    }

    // Checks if this optional value contains data.
    CELL_NODISCARD CELL_FUNCTION_INTERNAL bool IsValid() const {
        return this->hasData;
    }

    // Allows assigning new data to fill this optional value.
    CELL_FUNCTION_INTERNAL Optional<T>& operator =(const T& data) {
        this->data = data;
        this->hasData = true;

        return *this;
    }

    // Operator shortcut for IsValid.
    CELL_NODISCARD CELL_FUNCTION_INTERNAL operator bool() const {
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

template <typename T> constexpr Optional<T> None() {
    return Optional<T>();
}

}
