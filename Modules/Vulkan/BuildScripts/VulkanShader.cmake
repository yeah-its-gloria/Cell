# SPDX-FileCopyrightText: Copyright 2023 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

find_package(glslc REQUIRED FATAL_ERROR)

# Adds a shader to the build queue and makes the given target depend on it.
#
# Parameters:
# target  (Target): Name of the target that wants this shader.
# name    (String): Name of the shader. Can be something like "DefaultVertex" or "StuffModelFragment".
# input   (Path):   Path to the shader file that'll be compiled.
# output  (Path):   Path to where the shader will be written.
#
# Optional parameters:
# is_glsl (Bool):   Whether this shader is written in GLSL instead of HLSL.
function(CellVulkanAddShader target name input output)
    if(${ARGC} GREATER 3 AND "${ARGV4}" STREQUAL "TRUE") # Why do I use CMake
        set(type "GLSL")
        set(additional_shader_parameters "-x" "glsl")
    else()
        set(type "HLSL")
        set(additional_shader_parameters "-fhlsl-16bit-types" "-x" "hlsl")
    endif()

    cmake_path(RELATIVE_PATH output BASE_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE output_relative)

    add_custom_command(
        COMMENT "Building ${type} object ${output_relative}..."

        OUTPUT  ${output}
        DEPENDS ${input}

        COMMAND
            ${glslc_PROGRAM}
            "-mfmt=bin"
            "-O0"
            "--target-env=vulkan1.3"
            "--target-spv=spv1.4"
            ${additional_shader_parameters}
            "-Werror"
            ${input}
            "-o"
            ${output}
    )

    add_custom_target(CellShaders.${name} ALL DEPENDS ${output})
    add_dependencies(${target} CellShaders.${name})
endfunction()
