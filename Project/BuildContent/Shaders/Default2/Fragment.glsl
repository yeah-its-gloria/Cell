// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#version 460
#pragma shader_stage(fragment)

layout (location = 0) in vec4 inColor;
layout (location = 1) in vec2 inTexCoords;
layout (location = 2) in flat uint inTexIndex;

layout (location = 0) out vec4 outColor;

layout (set = 0, binding = 1) uniform sampler2D texSampler1;
layout (set = 0, binding = 2) uniform sampler2D texSampler2;

void main() {
    outColor = inColor * (inTexIndex == 0 ? texture(texSampler1, inTexCoords) : texture(texSampler2, inTexCoords));
}
