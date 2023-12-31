// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma shader_stage(fragment)

#include "Shared.hlslh"

Texture2D    colorTexture1 : register(t1);
SamplerState colorSampler1 : register(s1);

Texture2D    colorTexture2 : register(t2);
SamplerState colorSampler2 : register(s2);

float4 sampleTex(uint index, float2 coords) {
    switch (index) {
    case 0:
        return colorTexture1.Sample(colorSampler1, coords);

    case 1:
        return colorTexture2.Sample(colorSampler2, coords);

    default:
        break;
    }

    return 1.0;
}

float4 fragMain(FragmentShaderInput input) : SV_TARGET {
    return input.color * sampleTex(input.texIndex, input.texCoords);
}
