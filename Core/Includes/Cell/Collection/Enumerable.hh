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

}
