# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

find_package(dxc REQUIRED FATAL_ERROR)

# Adds a shader to the build queue and makes the given target depend on it.
#
# Parameters:
# target     (Target): Name of the target that wants this shader.
# name       (String): Name of the shader. Can be something like "Default", "DefaultVertex" or "StuffModelFragment".
# input      (Path):   Path to the shader file that'll be compiled.
# output_dir (Path):   Path to the folder where the shader will be stored.
# type       (String): Type of the shader: VERTEX, PIXEL
function(CellD3D12AddShader target name input output_dir type)
    set(output "${output_dir}/${name}.dxil")

    if (${type} STREQUAL "VERTEX")
        set(entry "VertexMain")
        set(profile "vs_6_0")
    elseif (${type} STREQUAL "PIXEL")
        set(entry "PixelMain")
        set(profile "ps_6_0")
    else ()
        message(FATAL_ERROR "Invalid shader type ${type}.")
    endif ()

    cmake_path(RELATIVE_PATH output BASE_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE output_relative)

    add_custom_command(
        COMMENT "Building HLSL (D3D12) object ${output_relative}..."

        OUTPUT  ${output}
        DEPENDS ${input}

        COMMAND
            ${dxc_PROGRAM}
            "-T"
            ${profile}
            "-E"
            ${entry}
            "-WX"
            ${input}
            "-Fo"
            ${output}
    )

    add_custom_target(CellD3D12Shaders.${name} ALL DEPENDS ${output})
    add_dependencies(${target} CellD3D12Shaders.${name})
endfunction()
