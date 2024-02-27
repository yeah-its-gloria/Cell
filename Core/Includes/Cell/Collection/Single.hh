// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Collection/Enumerable.hh>

namespace Cell::Collection {

// Shim template to allow single elements to present as enumerable.
template <typename T> class Single : public IEnumerable<T> {
public:
    // Creates a new array from the given block, and assumes its size.
    CELL_INLINE constexpr Single(T& ref) : element(ref) { }

    // Defaulted destructor.
    CELL_INLINE constexpr ~Single() = default;

    // Returns the number of elements in the block.
    CELL_NODISCARD CELL_INLINE size_t GetCount() const override {
        return 1;
    }

    // Returns the element at the index within the block.
    CELL_NODISCARD CELL_INLINE T& operator [] (const size_t index) override {
        CELL_ASSERT(index < 1);

        return this->element;
    }

    // Returns the element at the index within the block.
    CELL_NODISCARD CELL_INLINE const T& operator [] (const size_t index) const override {
        CELL_ASSERT(index < 1);

        return this->element;
    }

    // Returns the start address of the block.
    CELL_NODISCARD CELL_INLINE T* begin() override {
        return &this->element;
    }

    // Returns the end address of the block.
    CELL_NODISCARD CELL_INLINE T* end() override {
        return &this->element;
    }

    // Returns the start address of the block.
    CELL_NODISCARD CELL_INLINE const T* begin() const override {
        return &this->element;
    }

    // Returns the end address of the block.
    CELL_NODISCARD CELL_INLINE const T* end() const override {
        return &this->element;
    }

private:
    T& element;
};

}
