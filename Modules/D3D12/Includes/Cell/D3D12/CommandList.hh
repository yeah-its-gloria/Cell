// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/D3D12/Pipeline.hh>
#include <Cell/D3D12/Resource.hh>
#include <Cell/D3D12/Swapchain.hh>

namespace Cell::D3D12 {

// Represents a list of commands.
class CommandList : public Object {
friend Device;

public:
    // Destroys this command list.
    CELL_FUNCTION ~CommandList();

    // Sorry
    CELL_FUNCTION void StupidTestThing(Swapchain* CELL_NONNULL swapchain, Resource* CELL_NONNULL vertices);

    CELL_NON_COPYABLE(CommandList)

private:
    CELL_FUNCTION_INTERNAL CELL_INLINE CommandList(Device* d, Pipeline* p, ID3D12GraphicsCommandList* l) : device(d), pipeline(p), list(l) { }

    Device* device;
    Pipeline* pipeline;

    ID3D12GraphicsCommandList* list;
};

}
