// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>
#include <Cell/Optional.hh>

namespace Cell::Collection {

// Enumerable interface for list like objects.
template <typename T> class IEnumerable : public Object {
public:
    virtual ~IEnumerable() = default;

    // Retrieves the count of elements in this enumerable.
    CELL_NODISCARD virtual size_t GetCount() const = 0;

    // Retrieves the element at the given index.
    CELL_NODISCARD virtual T& operator [] (const size_t index) = 0;

    // Begin operator for foreach operations.
    CELL_NODISCARD virtual T* begin() = 0;

    // End operator for foreach operations.
    CELL_NODISCARD virtual T* end() = 0;

    // Checks whether this enumerable contains no items.
    CELL_NODISCARD bool IsEmpty() const {
        return this->GetCount() == 0;
    }
};

// Shim template for a bare array as an enumerable.
template <typename T> class Array : public IEnumerable<T> {
public:
    CELL_INLINE Array(T* reference, const size_t count) : reference(reference), count(count) { }
    CELL_INLINE ~Array() { }

    CELL_NODISCARD size_t GetCount() const {
        return this->count;
    }

    CELL_NODISCARD T& operator [] (const size_t index) {
        CELL_ASSERT(index < this->count);

        return this->reference[index];
    }

    CELL_NODISCARD T* begin() {
        return this->reference;
    }

    CELL_NODISCARD T* end() {
        return this->reference + (this->count - 1);
    }

private:
    T* reference;
    size_t count;
};

}
