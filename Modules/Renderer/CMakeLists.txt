# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(CellRenderer LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Cell - Modules - Renderer")

set(module_include_dir ${CMAKE_CURRENT_LIST_DIR}/Includes)

add_library(CellRenderer ${CELL_LIBRARY_TYPE}
    ${module_include_dir}/Cell/Renderer/Manager.hh
    ${module_include_dir}/Cell/Renderer/Result.hh

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Stub.cc
)

target_compile_definitions(CellRenderer PUBLIC "CELL_MODULES_RENDERER_AVAILABLE=1")
target_include_directories(CellRenderer PUBLIC ${module_include_dir})

target_link_libraries(CellRenderer PUBLIC
    CellCore
    CellMathematics
    CellShell
)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Backends/Vulkan)
set(cell_renderer_vulkan_enabled TRUE PARENT_SCOPE)

if (WIN32)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Backends/D3D12)
    set(cell_renderer_d3d12_enabled TRUE PARENT_SCOPE)
else ()
    set(cell_renderer_d3d12_enabled FALSE PARENT_SCOPE)
endif ()

if (APPLE)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Backends/Metal)
    set(cell_renderer_metal_enabled TRUE PARENT_SCOPE)
else ()
    set(cell_renderer_metal_enabled FALSE PARENT_SCOPE)
endif ()

cell_target_bootstrap(CellRenderer)
