// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Memory.hh>
#include <Cell/System/Panic.hh>

#include <stdlib.h>
#include <string.h>

namespace Cell::System {

void* AllocateMemory(const size_t size) {
    CELL_ASSERT(size > 0);

    void* buffer = calloc(1, size);
    CELL_ASSERT(buffer != nullptr);

    return buffer;
}

void ReallocateMemory(void** buffer, const size_t size) {
    CELL_ASSERT(size > 0);

    *buffer = realloc(*buffer, size);
    CELL_ASSERT(buffer != nullptr);
}

void FreeMemory(const void* buffer) {
    free((void*)buffer);
}

void CopyMemory(void* destination, const void* source, const size_t size) {
    memcpy(destination, source, size);
}

bool CompareMemory(const void* a, const void* b, const size_t size) {
    return memcmp(a, b, size) == 0;
}

void ClearMemory(void* block, const size_t size) {
    memset(block, 0, size);
}

}
