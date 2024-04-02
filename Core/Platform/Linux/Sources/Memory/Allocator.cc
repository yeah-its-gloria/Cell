// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Memory/Allocator.hh>
#include <Cell/System/Panic.hh>

#include <stdlib.h>
#include <string.h>

namespace Cell::Memory {

void* Allocate(const size_t size) {
    CELL_ASSERT(size > 0);

    void* buffer = calloc(1, size);
    CELL_ASSERT(buffer != nullptr);

    return buffer;
}

void Reallocate(void*& buffer, const size_t size) {
    CELL_ASSERT(size > 0);

    buffer = realloc(buffer, size);
    CELL_ASSERT(buffer != nullptr);
}

void Free(const void* buffer) {
    free((void*)buffer);
}

void Copy(void* destination, const void* source, const size_t size) {
    memcpy(destination, source, size);
}

bool Compare(const void* a, const void* b, const size_t size) {
    return memcmp(a, b, size) == 0;
}

void Clear(void* block, const size_t size) {
    memset(block, 0, size);
}

}
