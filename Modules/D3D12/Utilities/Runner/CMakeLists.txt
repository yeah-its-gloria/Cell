# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(CellD3D12Runner LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Cell - D3D12 - Runner Utility")

add_executable(CellD3D12Runner ${CMAKE_CURRENT_LIST_DIR}/Sources/Main.cc)

target_link_libraries(CellD3D12Runner PRIVATE CellCore CellD3D12)

CellPlatformBootstrapTarget(CellD3D12Runner TRUE)
