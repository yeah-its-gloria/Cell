// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Wrapped.hh>
#include <Cell/Renderer/Result.hh>
#include <Cell/Shell//Implementations/macOS.hh>

#ifdef __OBJC__
#include <Metal/Metal.h>
#endif

namespace Cell::Renderer::Metal {

// Represents a Metal rendering device.
class Device : public NoCopyObject {
public:
    // Finds the most suitable device and initializes it.
    CELL_FUNCTION static Wrapped<Device*, Result> New(Shell::Implementations::macOS* CELL_NONNULL shell);

    // Wrapper for IShell to create a device.
    CELL_FUNCTION_TEMPLATE static Wrapped<Device*, Result> New(Shell::IShell* CELL_NONNULL shell) {
        return New((Shell::Implementations::macOS*)shell);
    }

    // Cleans up all resources and destroys this device.
    CELL_FUNCTION ~Device();

private:
#ifdef __OBJC__
    CELL_FUNCTION_INTERNAL Device(id<MTLDevice> d) : device(d) { }
#else
    CELL_FUNCTION_INTERNAL Device(void* d) : device(d) { }
#endif

#ifdef __OBJC__
    id<MTLDevice> device;
#else
    void* device;
#endif
};

}
