// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>
#include <Cell/Optional.hh>

namespace Cell::Collection {

// Enumerable interface for list like objects.
template <typename T> class IEnumerable : public Object {
public:
    // Retrieves the count of elements in this enumerable.
    CELL_NODISCARD virtual size_t GetCount() const = 0;

    // Retrieves the element at the given index.
    CELL_NODISCARD virtual T& operator [] (const size_t index) = 0;

    // Retrieves the element at the given index.
    CELL_NODISCARD virtual const T& operator [] (const size_t index) const = 0;

    // Begin operator for foreach operations.
    CELL_NODISCARD virtual T* begin() = 0;

    // End operator for foreach operations.
    CELL_NODISCARD virtual T* end() = 0;

    // Begin operator for constant foreach operations.
    CELL_NODISCARD virtual const T* begin() const = 0;

    // End operator for constant foreach operations.
    CELL_NODISCARD virtual const T* end() const = 0;

    // Checks whether this enumerable contains no items.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE bool IsEmpty() const {
        return this->GetCount() == 0;
    }
};

}
