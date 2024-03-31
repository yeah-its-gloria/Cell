// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell {

// Object reference wrapper.
template <typename T> class Reference : public Object {
public:
    // Creates a dead reference.
    CELL_FUNCTION_INTERNAL constexpr Reference() : object(nullptr) { }

    // Creates a new reference.
    CELL_FUNCTION_INTERNAL constexpr Reference(T& object) : object(__builtin_addressof(object)) { }

    // Unwraps the reference.
    CELL_NODISCARD CELL_FUNCTION_INTERNAL constexpr T& Unwrap() {
        return *this->object;
    }

    CELL_NODISCARD CELL_FUNCTION_INTERNAL constexpr const T& Unwrap() const {
        return *this->object;
    }

    // Acts directly upon the referenced object.
    CELL_NODISCARD CELL_FUNCTION_INTERNAL constexpr T& operator -> () {
        return this->Unwrap();
    }

private:
    T* object;
};

}
