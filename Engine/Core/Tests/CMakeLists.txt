# SPDX-FileCopyrightText: Copyright 2023 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(CellCoreTests LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Cell - Tests")

function(cell_add_test name)
    add_executable(CellCore${name}Test
                   ${CMAKE_CURRENT_LIST_DIR}/Sources/${name}.cc
    )

    target_link_libraries(CellCore${name}Test PRIVATE CellCore)

    add_test(NAME CellCore${name}Test COMMAND CellCore${name}Test WORKING_DIRECTORY ${PROJECT_ROOT_DIR})

    CellPlatformBootstrapTarget(CellCore${name}Test TRUE)
endfunction()

cell_add_test(List)
cell_add_test(System)
cell_add_test(Utilities)
