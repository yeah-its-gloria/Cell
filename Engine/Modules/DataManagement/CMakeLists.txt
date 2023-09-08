# SPDX-FileCopyrightText: Copyright 2023 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(CellModulesDataManagement LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Cell - Modules - Data Management")

set(MODULE_INCLUDES_DIR ${CMAKE_CURRENT_LIST_DIR}/Includes)

add_library(CellModulesDataManagement ${CELL_LIBRARY_TYPE}
            ${MODULE_INCLUDES_DIR}/Cell/DataManagement/Archive.hh
            ${MODULE_INCLUDES_DIR}/Cell/DataManagement/Container.hh
            ${MODULE_INCLUDES_DIR}/Cell/DataManagement/Result.hh

            ${MODULE_INCLUDES_DIR}/Cell/DataManagement/Foreign/Base64.hh
            ${MODULE_INCLUDES_DIR}/Cell/DataManagement/Foreign/glTF.hh
            ${MODULE_INCLUDES_DIR}/Cell/DataManagement/Foreign/HTTP.hh
            ${MODULE_INCLUDES_DIR}/Cell/DataManagement/Foreign/JSON.hh
            ${MODULE_INCLUDES_DIR}/Cell/DataManagement/Foreign/PNG.hh

            ${CMAKE_CURRENT_LIST_DIR}/Sources/Archive.cc

            ${CMAKE_CURRENT_LIST_DIR}/Sources/Foreign/CRC.hh
            ${CMAKE_CURRENT_LIST_DIR}/Sources/Foreign/Base64.cc
            ${CMAKE_CURRENT_LIST_DIR}/Sources/Foreign/glTF.cc
            ${CMAKE_CURRENT_LIST_DIR}/Sources/Foreign/HTTP.cc
            ${CMAKE_CURRENT_LIST_DIR}/Sources/Foreign/HTTPSocket.cc
            ${CMAKE_CURRENT_LIST_DIR}/Sources/Foreign/JSON.cc
            ${CMAKE_CURRENT_LIST_DIR}/Sources/Foreign/PNG.cc
)

target_compile_definitions(CellModulesDataManagement PUBLIC "CELL_MODULES_DATAMANAGEMENT_AVAILABLE=1")
target_include_directories(CellModulesDataManagement PUBLIC ${MODULE_INCLUDES_DIR})
target_link_libraries(CellModulesDataManagement PUBLIC CellCore)

if (CELL_ENABLE_TESTS)
    add_executable(CellModulesDataManagementTestsParsing
                   ${CMAKE_CURRENT_LIST_DIR}/Tests/Parsing.cc
    )

    target_link_libraries(CellModulesDataManagementTestsParsing PUBLIC
                          CellCore
                          CellModulesDataManagement
    )

    add_test(NAME CellModulesDataManagementTestsParsing COMMAND CellModulesDataManagementTestsParsing WORKING_DIRECTORY ${PROJECT_ROOT_DIR})

    Cell_Platform_Bootstrap_Target(CellModulesDataManagementTestsParsing TRUE)
endif ()

Cell_Platform_Bootstrap_Target(CellModulesDataManagement)
