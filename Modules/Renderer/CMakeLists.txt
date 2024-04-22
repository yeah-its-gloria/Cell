# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(CellRenderer LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Cell - Modules - Renderer")

set(MODULE_INCLUDES_DIR ${CMAKE_CURRENT_LIST_DIR}/Includes)

add_library(CellRenderer ${CELL_LIBRARY_TYPE}
    ${MODULE_INCLUDES_DIR}/Cell/Renderer/Manager.hh
    ${MODULE_INCLUDES_DIR}/Cell/Renderer/Result.hh

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Stub.cc
)

target_compile_definitions(CellRenderer PUBLIC "CELL_MODULES_RENDERER_AVAILABLE=1")
target_include_directories(CellRenderer PUBLIC ${MODULE_INCLUDES_DIR})

target_link_libraries(CellRenderer
    PUBLIC CellCore
    PUBLIC CellMathematics
    PUBLIC CellShell
)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Backends/Vulkan)

if (WIN32)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Backends/D3D12)
endif ()

if (APPLE)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Backends/Metal)
endif ()

cell_target_bootstrap(CellRenderer)
