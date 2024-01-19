# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(CellD3D12 LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Cell - Modules - D3D12")

set(MODULE_INCLUDES_DIR ${CMAKE_CURRENT_LIST_DIR}/Includes)

add_library(CellD3D12 ${CELL_LIBRARY_TYPE}
    ${MODULE_INCLUDES_DIR}/Cell/D3D12/Device.hh

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Device.cc
)

target_compile_definitions(CellD3D12 PUBLIC "CELL_MODULES_D3D12_AVAILABLE=1")
target_compile_options(CellD3D12 PRIVATE -Wno-language-extension-token)
target_include_directories(CellD3D12 PUBLIC ${MODULE_INCLUDES_DIR})
target_link_libraries(CellD3D12 PUBLIC CellCore CellShell d3d12 dxgi)

if (NOT WIN32)
    message(FATAL_ERROR "Unsupported platform")
endif ()

CellPlatformBootstrapTarget(CellD3D12)

if (CELL_ENABLE_UTILITIES)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Utilities/Runner)
endif ()
