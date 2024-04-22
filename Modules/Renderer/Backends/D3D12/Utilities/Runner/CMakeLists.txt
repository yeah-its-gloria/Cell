# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

add_executable(CellRendererD3D12Runner
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Tools.hh

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Main.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Tools.cc
)

target_link_libraries(CellRendererD3D12Runner PRIVATE CellCore CellRenderer)
cell_target_bootstrap(CellRendererD3D12Runner TRUE)

CellRendererAddD3D12Shader(CellRendererD3D12Runner DefaultVertex ${CMAKE_CURRENT_LIST_DIR}/BuildContent/Default.hlsl ${CMAKE_CURRENT_LIST_DIR}/Content VERTEX)
CellRendererAddD3D12Shader(CellRendererD3D12Runner DefaultPixel ${CMAKE_CURRENT_LIST_DIR}/BuildContent/Default.hlsl ${CMAKE_CURRENT_LIST_DIR}/Content PIXEL)
