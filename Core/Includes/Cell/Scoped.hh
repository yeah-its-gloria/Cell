// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Memory/Allocator.hh>

namespace Cell {

// Scoped block of memory that will free itself using Cell::System::FreeMemory on expiry.
// It features no count or pre-allocation; this is only meant to be used for scoping heap memory.
template <typename T> class ScopedBlock : public NoCopyObject {
public:
    // Creates a scoped block.
    CELL_FUNCTION_TEMPLATE ScopedBlock(T* block) : block(block) { }

    // Destructor for the scoped block.
    CELL_FUNCTION_TEMPLATE ~ScopedBlock() { Memory::Free(this->block); }

    // Turns the managed block into a bare pointer.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE T* operator &() { return this->block; }

    // Turns the managed block into a bare pointer.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE operator T*() { return this->block; }

    // Turns the managed block into a bare pointer.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE operator void*() { return (void*)this->block; }

    // Array operator implementation acting upon the stored block.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE T operator[](const size_t index) { return this->block[index]; }

    // Overrides the stored block.
    CELL_FUNCTION_TEMPLATE ScopedBlock<T>& operator =(const T& block) {
        this->block = block;
        return *this;
    }

private:
    T* block;
};

// Template creating a scoped object.
template <typename T> class ScopedObject : public NoCopyObject {
public:
    // Creates a new scoped object.
    CELL_FUNCTION_TEMPLATE ScopedObject(T* object) : object(object) { }

    // Destructor for the scoped object.
    CELL_FUNCTION_TEMPLATE ~ScopedObject() { delete this->object; }

    // Returns the underlying pointer.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE T* GetPointer() { return this->object; }

    // Convenience operator to act directly upon the wrapped object.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE T* operator ->() { return this->object; }

    // Convenience operator to act directly upon the wrapped object.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE operator T*() { return this->object; }

    // Turns the managed object into a bare pointer.
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE T* operator &() { return this->object; }

    // Overrides the stored object.
    CELL_FUNCTION_TEMPLATE ScopedObject<T>& operator =(const T& object) {
        this->object = object;
        return *this;
    }

private:
    T* object;
};

}
