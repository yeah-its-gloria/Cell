// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Reference.hh>
#include <Cell/Shell/Input.hh>
#include <Cell/System/Platform.hh>

class Example : public Cell::Object {
public:
    CELL_FUNCTION_INTERNAL Example(Cell::System::IPlatform& platform) : platform(platform) {}

    CELL_INLINE ~Example() { delete input; }

    CELL_FUNCTION_INTERNAL void Launch(const Cell::System::String& parameterString);

private:
    CELL_FUNCTION_INTERNAL void AudioThread();
    CELL_FUNCTION_INTERNAL void NetworkThread();
    CELL_FUNCTION_INTERNAL void XRThread();
    CELL_FUNCTION_INTERNAL void VulkanThread();

    Cell::System::IPlatform& platform;

    Cell::Shell::Input* input = nullptr;

    // TODO: communications :tm:
};
