// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma shader_stage(vertex)

#include "Shared.hlslh"

struct VertexInput {
    [[vk::location(0)]]
    float3 position : POSITION0;

    [[vk::location(1)]]
    float4 color : COLOR0;

    [[vk::location(2)]]
    float2 texCoords : TEXCOORD;

    [[vk::location(3)]]
    uint texIndex;

    uint index : SV_VertexID;
};

struct VertexOutput {
    float4 position : SV_POSITION;

    FragmentInput fragInput;
};

cbuffer : register(b0) {
    struct {
        float4x4 model;
        float4x4 view;
        float4x4 projection;

        float delta;
    } MainUBO;
};

VertexOutput vertexMain(VertexInput input) {
    VertexOutput output = (VertexOutput)0;

    output.position = float4(input.position.xyz, 1.0) * MainUBO.model * MainUBO.view * MainUBO.projection;

    output.fragInput.color     = input.color;
    output.fragInput.texCoords = input.texCoords;
    output.fragInput.texIndex  = input.texIndex;

    return output;
}
