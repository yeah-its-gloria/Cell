# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(CellD3D12Runner LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Cell - Modules - D3D12 - Runner")

add_executable(CellD3D12Runner
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Tools.hh

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Main.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Tools.cc
)

target_link_libraries(CellD3D12Runner PRIVATE CellCore CellD3D12)
cell_target_bootstrap(CellD3D12Runner TRUE)

CellD3D12AddShader(CellD3D12Runner DefaultVertex ${CMAKE_CURRENT_LIST_DIR}/BuildContent/Default.hlsl ${CMAKE_CURRENT_LIST_DIR}/Content VERTEX)
CellD3D12AddShader(CellD3D12Runner DefaultPixel ${CMAKE_CURRENT_LIST_DIR}/BuildContent/Default.hlsl ${CMAKE_CURRENT_LIST_DIR}/Content PIXEL)
