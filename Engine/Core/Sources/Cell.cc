// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Cell.hh>
#include <Cell/System/Memory.hh>

namespace Cell {

void* Object::operator new(size_t size) {
    return System::AllocateMemory(size);
}

void Object::operator delete(void* memory, size_t size) {
    (void)(size);

    System::FreeMemory(memory);
}

}
