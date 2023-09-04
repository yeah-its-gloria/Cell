# SPDX-FileCopyrightText: Copyright 2023 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(OpenInput.Example LANGUAGES C CXX VERSION 1.0.0)

add_executable(OpenInput.Example
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Main.cc
)

target_link_libraries(OpenInput.Example PRIVATE CellCore OpenInput)

Cell_Platform_Bootstrap_Target(OpenInput.Example TRUE)
