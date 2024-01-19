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

private:
    uintptr_t handle;
};

// Template for an auto destructing mutex, holding a reference over a variable.
template <typename T> class AutoMutex : public Object {
public:
    AutoMutex(T& variable, Mutex& mutex) : variable(variable), mutex(mutex) { mutex.Lock(); }
    ~AutoMutex() { mutex.Unlock(); }

    operator T() { return this->variable; }
    T operator ->() { return this->variable; }

private:
    T& variable;
    Mutex& mutex;
};

}
