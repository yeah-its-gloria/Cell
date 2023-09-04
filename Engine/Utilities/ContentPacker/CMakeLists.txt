# SPDX-FileCopyrightText: Copyright 2023 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(CellUtilitiesContentPacker LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Cell - Utilities - Content Packer")

add_executable(CellUtilitiesContentPacker
               ${CMAKE_CURRENT_LIST_DIR}/Sources/Main.cc
)

target_link_libraries(CellUtilitiesContentPacker PRIVATE CellCore)

Cell_Platform_Bootstrap_Target(CellUtilitiesContentPacker TRUE)
