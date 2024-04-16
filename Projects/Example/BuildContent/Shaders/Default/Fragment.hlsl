// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
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

// fragMain
float4 main(FragmentInput input, bool isFront : SV_IsFrontFace) : SV_Target {
    float4 color = input.color * sampleTex(input.texIndex, input.texCoords);
    if (!isFront) {
        float average = (color.r + color.g + color.b) / 3.0;
        return float4(average, average, average, 1.0);
    }

    return color;
}
