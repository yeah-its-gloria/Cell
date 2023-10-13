# SPDX-FileCopyrightText: Copyright 2023 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(CellDataManagement LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Cell - Modules - Data Management")

option(CELL_DATAMANAGEMENT_TESTS_ENABLE_BROKEN "Enables currently broken tests." ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH};${CMAKE_CURRENT_LIST_DIR}/BuildScripts)

include(ContentPacking)

set(MODULE_INCLUDES_DIR ${CMAKE_CURRENT_LIST_DIR}/Includes)

add_library(CellDataManagement ${CELL_LIBRARY_TYPE}
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

target_compile_definitions(CellDataManagement PUBLIC "CELL_MODULES_DATAMANAGEMENT_AVAILABLE=1")
target_include_directories(CellDataManagement PUBLIC ${MODULE_INCLUDES_DIR})
target_link_libraries(CellDataManagement PUBLIC CellCore)

if (CELL_ENABLE_TESTS)
    function(cell_dm_add_test name)
        add_executable(CellDataManagementParsing${name}Test ${CMAKE_CURRENT_LIST_DIR}/Tests/${name}.cc)
        target_link_libraries(CellDataManagementParsing${name}Test PRIVATE CellDataManagement)
        add_test(NAME CellDataManagementParsing${name}Test COMMAND CellDataManagementParsing${name}Test WORKING_DIRECTORY ${PROJECT_ROOT_DIR})
        CellPlatformBootstrapTarget(CellDataManagementParsing${name}Test TRUE)
    endfunction()

if(CELL_DATAMANAGEMENT_TESTS_ENABLE_BROKEN)
    cell_dm_add_test(glTF)
    cell_dm_add_test(HTTP)
    cell_dm_add_test(JSON)
    cell_dm_add_test(PNG)
endif()
endif ()

CellPlatformBootstrapTarget(CellDataManagement)

if (CELL_ENABLE_UTILITIES)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Utilities/ContentPacker)
endif ()
