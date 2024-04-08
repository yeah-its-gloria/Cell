// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Optional.hh>

namespace Cell {

// Object + Result wrapper.
// The lifetime of the given object is not managed by this template.
template <typename T, typename R> class Wrapped : public Object {
public:
    // Creates a successful result with an object.
    CELL_FUNCTION_TEMPLATE constexpr Wrapped(T object) : object(object) { }

    // Creates a failure condition with no object.
    CELL_FUNCTION_TEMPLATE constexpr Wrapped(R result) : result(result) {
        CELL_ASSERT(result != R::Success);
    }

    CELL_FUNCTION_TEMPLATE constexpr ~Wrapped() { }

    // Returns the wrapped object.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE T Unwrap() {
        return this->object.Unwrap();
    }

    // Returns the wrapped object if it's valid, or returns other.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE T UnwrapOr(T other) {
        return this->object ? this->object.Unwrap() : other;
    }

    // Returns the wrapped result.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE R Result() const {
        return this->result;
    }

    // Returns whether this wrapped result failed, which means the object it holds is invalid.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE bool IsValid() const {
        return this->object;
    }

    // Returns whether this wrapped result failed, which means the object it holds is invalid.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE operator bool() const {
        return this->object;
    }

private:
    Optional<T> object;
    R result = R::Success;
};

}
