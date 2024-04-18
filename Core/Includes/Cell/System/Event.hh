// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>
#include <Cell/System/Result.hh>

namespace Cell::System {

// Represents an event. Can be signaled, useful as a semaphore for operation.
class Event : public NoCopyObject {
public:
    // Creates a new event.
    CELL_FUNCTION Event(const bool createSignaled = false);

    // Releases the event.
    CELL_FUNCTION ~Event();

    // Signals the event.
    CELL_FUNCTION void Signal();

    // Resets the event.
    CELL_FUNCTION void Reset();

    // Waits for the event to be signaled with the given timeout.
    // By default, it blocks forever until the event is signaled.
    CELL_FUNCTION Result Wait(const uint32_t timeoutMs = 0);

private:
    uintptr_t impl;
};

}
