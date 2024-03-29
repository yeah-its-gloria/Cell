// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Wrapped.hh>
#include <Cell/System/Result.hh>

namespace Cell::System {

// Secure random number generation implementation.
class RandomNumberGenerator : public Object {
public:
    // Creates a new generator.
    CELL_FUNCTION static Wrapped<RandomNumberGenerator*, Result> New();

    // Destructs the generator.
    CELL_FUNCTION ~RandomNumberGenerator();

    // Generates a random 64 bit value.
    CELL_FUNCTION Wrapped<uint64_t, Result> Generate();

    CELL_NON_COPYABLE(RandomNumberGenerator)

private:
    CELL_HANDLE_CONSTRUCTOR(RandomNumberGenerator)

    const uintptr_t handle;
};

}
