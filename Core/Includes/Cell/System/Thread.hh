// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/System/Result.hh>
#include "String.hh"

namespace Cell::System {

typedef void (* ThreadFunction)(void* CELL_NULLABLE parameter);

// Represents a thread from the OS.
class Thread : public Object {
public:
    // Spawns a new thread.
    CELL_FUNCTION explicit Thread(ThreadFunction CELL_NONNULL function, void* CELL_NULLABLE parameter = nullptr, const System::String& name = "");

    // Kills the thread.
    CELL_FUNCTION ~Thread();

    // Blocks execution until this thread finishes.
    // By default, it blocks forever.
    CELL_FUNCTION Result Join(uint32_t timeMilliseconds = 0) const;

    // Checks if the thread is still active.
    CELL_NODISCARD CELL_FUNCTION bool IsActive() const;

    // Sets the name for this thread.
    CELL_FUNCTION Result SetName(const System::String& name);

    // Yields execution to another thread.
    CELL_FUNCTION static void Yield();

private:
    uintptr_t handle;
};

}
