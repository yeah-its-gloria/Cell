// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Renderer/D3D12/Pipeline.hh>
#include <Cell/Renderer/D3D12/Resource.hh>
#include <Cell/Renderer/D3D12/Swapchain.hh>

namespace Cell::Renderer::D3D12 {

// Represents a list of commands.
class CommandList : public NoCopyObject {
friend Device;

public:
    // Destroys this command list.
    CELL_FUNCTION ~CommandList();

    // Sorry
    CELL_FUNCTION void StupidTestThing(Swapchain* CELL_NONNULL swapchain, Resource* CELL_NONNULL vertices);

private:
    CELL_FUNCTION_INTERNAL CommandList(Device* d, Pipeline* p, ID3D12GraphicsCommandList* l) : device(d), pipeline(p), list(l) { }

    Device* device;
    Pipeline* pipeline;

    ID3D12GraphicsCommandList* list;
};

}
