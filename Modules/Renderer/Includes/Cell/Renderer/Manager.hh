// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Wrapped.hh>
#include <Cell/Renderer/Result.hh>

namespace Cell::Renderer {

// TODO: actually design this interface lol

// Rendering environment manager interface.
class IManager : NoCopyObject {
public:
    virtual ~IManager() = default;

    // Creates a device using this manager.
    Wrapped<class IDevice*, Result> CreateDevice();
};

}
