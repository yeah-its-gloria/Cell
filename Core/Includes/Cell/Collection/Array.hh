// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Collection/Enumerable.hh>

namespace Cell::Collection {

// Shim template for a block of memory as an enumerable.
template <typename T> class Array : public IEnumerable<T> {
public:
    // Creates a new array from the given block and size.
    CELL_INLINE constexpr Array(T* block, const size_t count) : block(block), count(count) { }

    // Defaulted destructor.
    CELL_INLINE constexpr ~Array() = default;

    // Returns the number of elements in the block.
    CELL_NODISCARD CELL_INLINE size_t GetCount() const override {
        return this->count;
    }

    // Returns the element at the index within the block.
    CELL_NODISCARD CELL_INLINE T& operator [] (const size_t index) override {
        CELL_ASSERT(index < this->count);

        return this->block[index];
    }

    // Returns the start address of the block.
    CELL_NODISCARD CELL_INLINE T* begin() override {
        return this->block;
    }

    // Returns the end address of the block.
    CELL_NODISCARD CELL_INLINE T* end() override {
        return this->block + (this->count);
    }

private:
    T* block;
    size_t count;
};

}
