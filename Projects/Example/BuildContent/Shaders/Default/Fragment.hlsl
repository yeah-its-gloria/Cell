// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma shader_stage(fragment)

#include "Shared.hlslh"

Texture2D    colorTexture : register(t1);
SamplerState colorSampler : register(s1);

float4 main(FragmentShaderInput input) : SV_TARGET
{
    return input.color * colorTexture.Sample(colorSampler, input.texCoords);
}
