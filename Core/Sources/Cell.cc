// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Cell.hh>
#include <Cell/Memory/Allocator.hh>
#include <Cell/System/Panic.hh>

namespace Cell {

void* Object::operator new(size_t size) {
    return Memory::Allocate(size);
}

void Object::operator delete(void* memory, size_t size) {
    (void)(size);

    Memory::Free(memory);
}

}
