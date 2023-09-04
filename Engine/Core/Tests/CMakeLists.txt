# SPDX-FileCopyrightText: Copyright 2023 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(CellCoreTests LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Cell - Tests")

function(cell_add_test name)
    add_executable(CellCoreTests${name}
                   ${CMAKE_CURRENT_LIST_DIR}/Sources/${name}.cc
    )

    target_link_libraries(CellCoreTests${name} PRIVATE CellCore)

    add_test(NAME CellCoreTests${name} COMMAND CellCoreTests${name} WORKING_DIRECTORY ${PROJECT_ROOT_DIR})

    Cell_Platform_Bootstrap_Target(CellCoreTests${name} TRUE)
endfunction()

cell_add_test(List)
cell_add_test(Utilities)
