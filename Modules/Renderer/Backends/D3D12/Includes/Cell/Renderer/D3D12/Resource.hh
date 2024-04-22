// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Renderer/D3D12/Device.hh>

namespace Cell::Renderer::D3D12 {

// TODO: make this less vertex buffer specific lol

// Represents a vertex buffer resource on the device that created it.
class Resource : public NoCopyObject {
friend class CommandList;
friend Device;

public:
    // Frees the resource's contents and destroys its instance.
    CELL_FUNCTION ~Resource();

    // Copies data into the buffer.
    CELL_FUNCTION Result CopyData(const Memory::IBlock& data);

private:
    CELL_FUNCTION_INTERNAL Resource(Device* d, ID3D12Resource* r, D3D12_VERTEX_BUFFER_VIEW v) : device(d), resource(r), view(v) { }

    Device* device;

    ID3D12Resource* resource;
    const D3D12_VERTEX_BUFFER_VIEW view;
};
}
