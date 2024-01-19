// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell {

// Object reference wrapper.
template <typename T> class Reference : public Object {
public:
    // Creates a new reference.
    CELL_INLINE Reference(T& object) : object(__builtin_addressof(object)) { }

    // Empty destructor.
    CELL_INLINE ~Reference() { }

    // Unwraps the reference.
    CELL_NODISCARD CELL_INLINE T& Unwrap() {
        return *this->object;
    }

    // Acts directly upon the referenced object.
    CELL_NODISCARD CELL_INLINE T& operator ->() {
        return this->Unwrap();
    }

private:
    T* object;
};

}
