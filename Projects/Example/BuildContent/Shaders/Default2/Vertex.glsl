// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#version 460
#pragma shader_stage(vertex)
#extension GL_EXT_scalar_block_layout : enable

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec4 inColor;
layout (location = 2) in vec2 inTexCoords;
layout (location = 3) in uint inTexIndex;

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec2 outTexCoords;
layout (location = 2) out uint outTexIndex;

layout (scalar, set = 0, binding = 0) uniform vsubo {
    mat4x4 model;
    mat4x4 view;
    mat4x4 projection;
    float delta;
};

float oscillate(float x) {
    return mod(x, 1.0) * (1.0 - 2.0 * mod(floor(x), 2.0)) + mod(floor(x), 2.0);
}

void main() {
    gl_Position = projection * view * model * vec4(inPosition.xyz, 1.0);

    outColor = inColor;
    outTexCoords = inTexCoords;
    outTexIndex = inTexIndex;
}
