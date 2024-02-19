// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/System/Memory.hh>

namespace Cell {

// Scoped block of memory that will free itself using Cell::System::FreeMemory on expiry.
// It features no count or pre-allocation; this is only meant to be used for scoping heap memory.
template <typename T> class ScopedBlock : public Object {
public:
    // Creates a scoped block.
    CELL_INLINE ScopedBlock(T* block) : block(block) { }

    // Destructor for the scoped block.
    CELL_INLINE ~ScopedBlock() { System::FreeMemory(this->block); }

    // Turns the managed block into a bare pointer.
    CELL_NODISCARD CELL_INLINE T* operator &() { return this->block; }

    // Turns the managed block into a bare pointer.
    CELL_NODISCARD CELL_INLINE operator T*() { return this->block; }

    // Overrides the stored block.
    CELL_INLINE ScopedBlock<T>& operator =(const T& block) {
        this->block = block;
        return *this;
    }

    CELL_NON_COPYABLE(ScopedBlock)

private:
    T* block;
};

// Template creating a scoped object.
template <typename T> class ScopedObject : public Object {
public:
    // Creates a new scoped object.
    CELL_INLINE ScopedObject(T* object) : object(object) { }

    // Destructor for the scoped object.
    CELL_INLINE ~ScopedObject() { delete this->object; }

    // Returns the underlying pointer.
    CELL_NODISCARD CELL_INLINE T* GetPointer() { return this->object; }

    // Convenience operator to act directly upon the wrapped object.
    CELL_NODISCARD CELL_INLINE T* operator ->() { return this->object; }

    // Convenience operator to act directly upon the wrapped object.
    CELL_NODISCARD CELL_INLINE operator T*() { return this->object; }

    // Turns the managed object into a bare pointer.
    CELL_NODISCARD CELL_INLINE T* operator &() { return this->object; }

    // Overrides the stored object.
    CELL_INLINE ScopedObject<T>& operator =(const T& object) {
        this->object = object;
        return *this;
    }

    CELL_NON_COPYABLE(ScopedObject)

private:
    T* object;
};

}
