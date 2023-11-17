// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/System/Block.hh>
#include <Cell/System/Memory.hh>
#include <Cell/System/Panic.hh>

namespace Cell::System {

// Represents a typed block of memory that owns its memory.
template <typename T> class OwnedBlock : public IBlock {
public:
    // Creates an owned, zeroed block of memory, with the given number of T elements allocated.
    CELL_INLINE explicit OwnedBlock(const size_t count) : data(nullptr), count(count) {
        this->data = System::AllocateMemory<T>(count);
    }

    // Destructs the block and frees its memory.
    CELL_INLINE ~OwnedBlock() {
        System::FreeMemory(this->data);
    }

    // Resizes the block to the given number of T elements.
    CELL_INLINE void Resize(const size_t count) {
        this->count = count;

        System::ReallocateMemory<T>(&this->data, this->count);
    }

    // Automatic pointer conversion utility.
    CELL_INLINE operator T* CELL_NONNULL() { return this->data; }

    // Automatic pointer conversion utility.
    CELL_INLINE operator const T* CELL_NONNULL() const { return (const uint8_t*)this->data; }

// -> IBlock

    CELL_NODISCARD CELL_INLINE void* Pointer() override { return this->data; }
    CELL_NODISCARD CELL_INLINE const void* Pointer() const override { return this->data; }
    CELL_NODISCARD CELL_INLINE size_t Count() const override { return this->count; }
    CELL_NODISCARD CELL_INLINE size_t BlockSize() const override { return sizeof(T); }

private:
    T* data;
    size_t count;
};

// Typed block wrapper for unowned memory.
template <typename T> class UnownedBlock : public IBlock {
public:
    // Creates an unowned block of memory with the given block address and element count.
    CELL_INLINE explicit UnownedBlock(const T* block, const size_t count = 1) : data(block), count(count) { }

// IBlock
    CELL_NODISCARD CELL_INLINE void* Pointer() override { return (void*)this->data; }
    CELL_NODISCARD CELL_INLINE const void* Pointer() const override { return this->data; }
    CELL_NODISCARD CELL_INLINE size_t Count() const override { return this->count; }
    CELL_NODISCARD CELL_INLINE size_t BlockSize() const override { return sizeof(T); }

private:
    const T* data;
    size_t count;
};

}
