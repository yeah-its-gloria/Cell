// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/String.hh>
#include <Cell/D3D12/Pipeline.hh>

CELL_FUNCTION_INTERNAL Cell::D3D12::Pipeline* D3D12ToolsLoadPipeline(const Cell::String& vertexPath, const Cell::String& pixelPath, Cell::D3D12::Device* device);
