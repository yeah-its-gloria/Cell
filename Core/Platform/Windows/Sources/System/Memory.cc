// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Memory.hh>
#include <Cell/System/Panic.hh>
#include <Cell/System/Platform/Windows/Includes.h>

namespace Cell::System {

void* AllocateMemory(const size_t size) {
    if (size == 0) {
        CELL_DEBUGBREAK;
    }

    CELL_ASSERT(size > 0);

    void* buffer = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
    CELL_ASSERT(buffer != nullptr);

    return buffer;
}

void ReallocateMemory(void** buffer, const size_t size) {
    CELL_ASSERT(size > 0);

    *buffer = HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, *buffer, size);
    CELL_ASSERT(buffer != nullptr);
}

void FreeMemory(const void* buffer) {
    const BOOL result = HeapFree(GetProcessHeap(), 0, (void*)buffer);
    CELL_ASSERT(result);
}

void CopyMemory(void* destination, const void* source, const size_t size) {
    CELL_ASSERT(size > 0);

    memcpy(destination, source, size);
}

bool CompareMemory(const void* a, const void* b, const size_t size) {
    CELL_ASSERT(size > 0);

    return memcmp(a, b, size) == 0;
}

void ClearMemory(void* block, const size_t size) {
    memset(block, 0, size);
}

}
