#!/bin/sh

# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

glslc -mfmt=bin -O --target-env=vulkan1.2 --target-spv=spv1.5 -fentry-point=main -fhlsl-16bit-types -x hlsl -Werror BuildContent/Shaders/Default/Vertex.hlsl -o Content/Shaders/DefaultVertex.spv
glslc -mfmt=bin -O --target-env=vulkan1.2 --target-spv=spv1.5 -fentry-point=main -fhlsl-16bit-types -x hlsl -Werror BuildContent/Shaders/Default/Fragment.hlsl -o Content/Shaders/DefaultFragment.spv

glslc -mfmt=bin -O --target-env=vulkan1.2 --target-spv=spv1.5 -fentry-point=main -x glsl -Werror BuildContent/Shaders/Default2/Vertex.glsl -o Content/Shaders/Default2Vertex.spv
glslc -mfmt=bin -O --target-env=vulkan1.2 --target-spv=spv1.5 -fentry-point=main -x glsl -Werror BuildContent/Shaders/Default2/Fragment.glsl -o Content/Shaders/Default2Fragment.spv

# spirv-link Content/Shaders/DefaultVertex.spv Content/Shaders/DefaultFragment.spv -o Content/Shaders/Default.spv
