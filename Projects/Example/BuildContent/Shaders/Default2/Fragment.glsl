// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#version 460
#pragma  shader_stage(fragment)

layout(location = 0) in vec4  inColor;
layout(location = 1) in vec2  inTexCoords;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D texSampler;

void main()
{
    outColor = inColor * texture(texSampler, inTexCoords);
}
