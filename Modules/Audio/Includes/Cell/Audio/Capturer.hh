// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Audio/Subsystem.hh>

namespace Cell::Audio {

// Base audio capturing interface.
class ICapturer : public Object {
public:
    virtual ~ICapturer() = default;

    // Starts capturing.
    virtual Result Start() = 0;

    // Stops capturing.
    virtual Result Stop() = 0;

    // Fetches the number of currently available samples.
    virtual Wrapped<uint32_t, Result> GetAvailableSampleCount() = 0;

    // Fetches the currently available data into the given block.
    virtual Wrapped<uint8_t*, Result> Fetch(const uint32_t count) = 0;
};

}
