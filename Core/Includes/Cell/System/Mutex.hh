// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::System {

// Represents a locking mechanism for resources. Useful to prevent data races.
class Mutex : public Object {
public:
    // Creates a new mutex.
    CELL_FUNCTION Mutex(bool createLocked = false);

    // Releases the mutex.
    CELL_FUNCTION ~Mutex();

    // Waits and locks the mutex.
    CELL_FUNCTION void Lock();

    // Unlocks the mutex.
    CELL_FUNCTION void Unlock();

    CELL_NON_COPYABLE(Mutex)

private:
    uintptr_t handle;
};

}
