# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

find_package(glslc REQUIRED FATAL_ERROR)
find_package(spirv-link REQUIRED FATAL_ERROR)

# Adds a shader to the build queue and makes the given target depend on it.
#
# Parameters:
# target     (Target): Name of the target that wants this shader.
# name       (String): Name of the shader. Can be something like "DefaultVertex" or "StuffModelFragment".
# input      (Path):   Path to the shader file that'll be compiled.
# output_dir (Path):   Path to the folder where the shader will be stored.
# type       (String): Type of the shader: vertex, fragment
#
# Optional parameters:
# is_glsl (Bool):   Whether this shader is written in GLSL instead of HLSL.
function(CellVulkanAddShader target name input output_dir type)
    set(output "${output_dir}/${name}.spv")

    if (${type} STREQUAL "VERTEX")
        set(entry "vertexMain")
    elseif (${type} STREQUAL "FRAGMENT")
        set(entry "fragMain")
    else ()
        message(FATAL_ERROR "Invalid shader type ${type}.")
    endif ()

    if (${ARGC} GREATER 4 AND "${ARGV5}" STREQUAL "TRUE") # Why do I use CMake
        set(lang "GLSL")
        set(additional_shader_parameters "-x" "glsl")
    else ()
        set(lang "HLSL")
        set(additional_shader_parameters "-fhlsl-16bit-types" "-x" "hlsl")
    endif ()

    cmake_path(RELATIVE_PATH output BASE_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE output_relative)

    add_custom_command(
        COMMENT "Building ${lang} object ${output_relative}..."

        OUTPUT  ${output}
        DEPENDS ${input}

        COMMAND
            ${glslc_PROGRAM}
            "-mfmt=bin"
            "-O"
            "--target-env=vulkan1.3"
            "--target-spv=spv1.6"
            "-fentry-point=${entry}"
            ${additional_shader_parameters}
            "-Werror"
            ${input}
            "-o"
            ${output}
    )

    add_custom_target(CellShaders.${name} ALL DEPENDS ${output})
    add_dependencies(${target} CellShaders.${name})
endfunction()

# Combines two shader stages into one SPIR-V binary.
#
# Parameters:
# target     (Target): Name of the target that wants this shader.
# name       (String): Name of the shader. Can be something like "Default" or "StuffModel".
# shader1    (Path):   Path to the (preferably) vertex shader.
# shader2    (Path):   Path to the (preferably) fragment shader.
# output_dir (Path):   Path to the folder where the shader will be stored.
function(CellVulkanLinkShaders target name shader1 shader2 output_dir)
    set(output "${output_dir}/${name}.spv")
    cmake_path(RELATIVE_PATH output BASE_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE output_relative)

    add_custom_command(
        COMMENT "Linking SPIR-V binary ${output_relative}..."

        OUTPUT  ${output}
        DEPENDS ${shader1} ${shader2}

        COMMAND
            ${spirv-link_PROGRAM}
            ${shader1}
            ${shader2}
            "-o"
            ${output}
    )

    add_custom_target(CellShadersLinked.${name} ALL DEPENDS ${output})
    add_dependencies(${target} CellShadersLinked.${name})
endfunction()
