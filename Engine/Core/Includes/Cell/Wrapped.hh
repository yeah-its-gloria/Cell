// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Optional.hh>
#include <Cell/System/Panic.hh>

namespace Cell {

// Object + Result wrapper.
template <typename T, typename R> class Wrapped : public Object {
public:
    // Creates a successful result with an object.
    CELL_INLINE Wrapped(T object) : object(object) { }

    // Creates a failure condition with no object.
    CELL_INLINE Wrapped(R result) : result(result) { }

    // Empty destructor.
    // Wrapped results do NOT handle the lifetime of the contained object.
    // The caller is responsible for this. Remember to check out ScopedObject.
    CELL_INLINE ~Wrapped() { }

    // Returns the wrapped object.
    CELL_NODISCARD CELL_INLINE T Unwrap() const {
        return this->object.Unwrap();
    }

    // Returns the wrapped result.
    CELL_NODISCARD CELL_INLINE R Result() const {
        return this->result;
    }

    // Returns whether this wrapped result failed, which means the object it holds is invalid.
    CELL_NODISCARD CELL_INLINE bool IsValid() const {
        return this->object;
    }

    // Returns whether this wrapped result failed, which means the object it holds is invalid.
    CELL_NODISCARD CELL_INLINE operator bool() const {
        return this->object;
    }

private:
    Optional<T> object;
    R result = R::Success;
};

}
