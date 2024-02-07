# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(CellEditor LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Cell - Editor")

set(CONTENT_DIR ${CMAKE_CURRENT_LIST_DIR}/Content)
set(SOURCES_DIR ${CMAKE_CURRENT_LIST_DIR}/Sources)

add_executable(CellEditor
    ${SOURCES_DIR}/Editor.hh
    ${SOURCES_DIR}/Editor.cc
)

target_link_libraries(CellEditor PRIVATE CellCore CellDataManagement CellShell)

cell_target_bootstrap(CellEditor)
