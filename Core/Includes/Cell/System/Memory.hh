// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::System {

// Allocates a size bytes large block.
CELL_FUNCTION void* CELL_NONNULL AllocateMemory(const size_t size);

// Reallocates the given block with the new byte size.
CELL_FUNCTION void ReallocateMemory(void* CELL_NONNULL* CELL_NONNULL block, const size_t size);

// Frees the given block.
CELL_FUNCTION void FreeMemory(const void* CELL_NONNULL block);

// Copies size bytes from the source address to the destination address.
CELL_FUNCTION void CopyMemory(void* CELL_NONNULL destination, const void* CELL_NONNULL source, const size_t size);

// Compares the data of the given blocks of memory and checks them for equality.
CELL_FUNCTION bool CompareMemory(const void* CELL_NONNULL a, const void* CELL_NONNULL b, const size_t size);

// Clears the given block to zero.
CELL_FUNCTION void ClearMemory(void* CELL_NONNULL block, const size_t size);

// Allocates a count * count bytes large block.
template <typename T> CELL_INLINE T* CELL_NONNULL AllocateMemory(const size_t count = 1) {
    return (T*)AllocateMemory(sizeof(T) * count);
}

// Reallocates the given block with the new count bytes * count.
template <typename T> CELL_INLINE void ReallocateMemory(T* CELL_NONNULL* CELL_NONNULL block, const size_t count) {
    ReallocateMemory((void**)block, sizeof(T) * count);
}

// Copies a block of memory to another.
template <typename T> CELL_INLINE void CopyMemory(T* CELL_NONNULL destination, const T* CELL_NONNULL source, const size_t count = 1) {
    CopyMemory((void*)destination, (const void*)source, sizeof(T) * count);
}

// Compares the data of the given blocks of memory and checks them for equality.
template <typename T> CELL_INLINE bool CompareMemory(const T* CELL_NONNULL a, const T* CELL_NONNULL b, const size_t count = 1) {
    return CompareMemory((const void*)a, (const void*)b, sizeof(T) * count);
}

// Clears the given block to zero.
template <typename T> CELL_INLINE void ClearMemory(T* CELL_NONNULL block, const size_t count = 1) {
    return ClearMemory((void*)block, sizeof(T) * count);
}

// Clears the given block to zero.
template <typename T> CELL_INLINE void ClearMemory(T& block, const size_t count = 1) {
    return ClearMemory((void*)&block, sizeof(T) * count);
}

}
