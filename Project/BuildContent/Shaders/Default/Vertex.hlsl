// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma shader_stage(vertex)

#include "Shared.hlslh"

struct VertexShaderOutput {
    float4 position : SV_POSITION;
    FragmentShaderInput fragmentInput;
};

cbuffer : register(b0) {
    float4x4 model;
    float4x4 view;
    float4x4 projection;
    float timeMs;
};

float oscillate(float x) {
    return (x % 1.0) * (1.0 - 2.0 * (floor(x) % 2.0)) + (floor(x) % 2.0);
}

VertexShaderOutput main ([[vk::location(0)]] float3 position : POSITION0,
                         [[vk::location(1)]] float4 color : COLOR0,
                         [[vk::location(2)]] float2 texCoords : TEXCOORD,
                         [[vk::location(3)]] uint texIndex
) {
    VertexShaderOutput output = (VertexShaderOutput)0;

    output.position = mul(projection, mul(view, mul(model, float4(position.xyz, 1.0))));
    output.fragmentInput.color = color;
    output.fragmentInput.texCoords = texCoords;
    output.fragmentInput.texIndex = texIndex;

    return output;
}
