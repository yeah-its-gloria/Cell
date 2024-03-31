// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::Memory {

// Allocates a size bytes large block.
CELL_FUNCTION void* CELL_NONNULL Allocate(const size_t size);

// Reallocates the given block with the new byte size.
CELL_FUNCTION void Reallocate(void* CELL_NONNULL & block, const size_t size);

// Frees the given block.
CELL_FUNCTION void Free(const void* CELL_NONNULL block);

// Copies size bytes from the source address to the destination address.
CELL_FUNCTION void Copy(void* CELL_NONNULL destination, const void* CELL_NONNULL source, const size_t size);

// Compares the data of the given blocks of memory and checks them for equality.
CELL_FUNCTION bool Compare(const void* CELL_NONNULL a, const void* CELL_NONNULL b, const size_t size);

// Clears the given block to zero.
CELL_FUNCTION void Clear(void* CELL_NONNULL block, const size_t size);

// Allocates a count * count bytes large block.
template <typename T> CELL_FUNCTION_TEMPLATE T* CELL_NONNULL Allocate(const size_t count = 1) {
    return (T*)Allocate(sizeof(T) * count);
}

// Reallocates the given block with the new count bytes * count.
template <typename T> CELL_FUNCTION_TEMPLATE void Reallocate(T* CELL_NONNULL & block, const size_t count) {
    Reallocate((void*&)block, sizeof(T) * count);
}

// Copies a block of memory to another.
template <typename T> CELL_FUNCTION_TEMPLATE void Copy(T* CELL_NONNULL destination, const T* CELL_NONNULL source, const size_t count = 1) {
    Copy((void*)destination, (const void*)source, sizeof(T) * count);
}

// Compares the data of the given blocks of memory and checks them for equality.
template <typename T> CELL_FUNCTION_TEMPLATE bool Compare(const T* CELL_NONNULL a, const T* CELL_NONNULL b, const size_t count = 1) {
    return Compare((const void*)a, (const void*)b, sizeof(T) * count);
}

// Clears the given block to zero.
template <typename T> CELL_FUNCTION_TEMPLATE void Clear(T* CELL_NONNULL block, const size_t count = 1) {
    return Clear((void*)block, sizeof(T) * count);
}

// Clears the given block to zero.
template <typename T> CELL_FUNCTION_TEMPLATE void Clear(T& block, const size_t count = 1) {
    return Clear((void*)&block, sizeof(T) * count);
}

}
