// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Collection/Enumerable.hh>

namespace Cell::Collection {

// Shim template for a block of memory as an enumerable.
template <typename T, size_t S> class Array : public IEnumerable<T> {
public:
    // Creates a new array from the given block, and assumes its size.
    CELL_INLINE constexpr Array(T (* blockPtr)[S]) : block(*blockPtr) { }

    // Defaulted destructor.
    CELL_INLINE constexpr ~Array() = default;

    // Returns the number of elements in the block.
    CELL_NODISCARD CELL_INLINE size_t GetCount() const override {
        return S;
    }

    // Returns the element at the index within the block.
    CELL_NODISCARD CELL_INLINE T& operator [] (const size_t index) override {
        CELL_ASSERT(index < S);

        return this->block[index];
    }

    // Returns the element at the index within the block.
    CELL_NODISCARD CELL_INLINE const T& operator [] (const size_t index) const override {
        CELL_ASSERT(index < S);

        return this->block[index];
    }

    // Returns the start address of the block.
    CELL_NODISCARD CELL_INLINE T* begin() override {
        return this->block;
    }

    // Returns the end address of the block.
    CELL_NODISCARD CELL_INLINE T* end() override {
        return this->block + S;
    }

    // Returns the start address of the block.
    CELL_NODISCARD CELL_INLINE const T* begin() const override {
        return this->block;
    }

    // Returns the end address of the block.
    CELL_NODISCARD CELL_INLINE const T* end() const override {
        return this->block + S;
    }

private:
    T* block;
};

}
