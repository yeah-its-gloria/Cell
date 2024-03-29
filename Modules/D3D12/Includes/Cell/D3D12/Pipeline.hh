// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/D3D12/Device.hh>

namespace Cell::D3D12 {

// Represents a compiled pipeline, with a vertex and pixel shader.
class Pipeline : public Object {
friend class CommandList;
friend Device;

public:
    // Destroys this pipeline and its cached resources.
    CELL_FUNCTION ~Pipeline();

    CELL_NON_COPYABLE(Pipeline)

private:
    CELL_FUNCTION_INTERNAL CELL_INLINE Pipeline(Device* d, ID3D12PipelineState* s) : device(d), state(s) { }

    Device* device;

    ID3D12PipelineState* state;
};

}
