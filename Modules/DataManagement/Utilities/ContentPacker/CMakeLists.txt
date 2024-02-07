# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(CellDataManagementContentPacker LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Cell - Data Management - Content Packer Utility")

add_executable(CellDataManagementContentPacker
               ${CMAKE_CURRENT_LIST_DIR}/Sources/Main.cc
)

target_link_libraries(CellDataManagementContentPacker PRIVATE CellCore CellDataManagement)

cell_target_bootstrap(CellDataManagementContentPacker TRUE)
