// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/String.hh>
#include <Cell/System/Result.hh>

// Shorthand for calling a class instance function in a thread.
//
// Requires the name of the class (as qualified as necessary for the context) and the name of the respective function.
// Also allows for supplying any needed parameters.
#define CELL_THREAD_CLASS_FUNC(name, function, ...) ([](void* p) { ((name*)p)->function(__VA_ARGS__); }), this

namespace Cell::System {

// Prototype definition for a thread function.
typedef void (* ThreadFunction)(void* CELL_NULLABLE parameter);

// Represents a thread from the OS.
class Thread : public NoCopyObject {
public:
    // Spawns a new thread, with the given parameter and, optionally, name.
    // Some platforms do not support names; they are only used for debugging facilities.
    CELL_FUNCTION explicit Thread(ThreadFunction CELL_NONNULL function, void* CELL_NULLABLE parameter = nullptr, const String& name = "");

    // Destructs the thread.
    // If the thread is still active, it will be terminated, if possible.
    CELL_FUNCTION ~Thread();

    // Blocks execution until this thread finishes, or the timeout in milliseconds expires.
    // By default, it blocks forever.
    CELL_FUNCTION Result Join(const uint32_t milliseconds = 0) const;

    // Returns whether the thread is still actively running.
    CELL_NODISCARD CELL_FUNCTION bool IsActive() const;

    // Sets the name of this thread, if possible.
    CELL_FUNCTION Result SetName(const String& name);

    // Requests the scheduler to yield execution.
    CELL_FUNCTION static void Yield();

private:
    uintptr_t impl;
};

}
