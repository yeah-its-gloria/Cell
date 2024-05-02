// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Mutex.hh>

#include <Foundation/Foundation.h>

namespace Cell::System {

Mutex::Mutex(const bool createLocked) {
    this->impl = (uintptr_t)[NSCondition new];

    if (createLocked) {
        [(NSCondition*)this->impl lock];
    }
}

Mutex::~Mutex() {
    /* ... */
}

void Mutex::Lock() {
    [(NSCondition*)this->impl lock];
}

void Mutex::Unlock() {
    [(NSCondition*)this->impl unlock];
}

}
