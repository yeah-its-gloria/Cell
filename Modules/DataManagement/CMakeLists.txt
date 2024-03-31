# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(CellDataManagement LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Cell - Modules - Data Management")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH};${CMAKE_CURRENT_LIST_DIR}/BuildScripts)

include(ContentPacking)

set(MODULE_INCLUDES_DIR ${CMAKE_CURRENT_LIST_DIR}/Includes)

add_library(CellDataManagement ${CELL_LIBRARY_TYPE}
    ${MODULE_INCLUDES_DIR}/Cell/DataManagement/Archive.hh
    ${MODULE_INCLUDES_DIR}/Cell/DataManagement/Base64.hh
    ${MODULE_INCLUDES_DIR}/Cell/DataManagement/Checksum.hh
    ${MODULE_INCLUDES_DIR}/Cell/DataManagement/Container.hh
    ${MODULE_INCLUDES_DIR}/Cell/DataManagement/HTTP.hh
    ${MODULE_INCLUDES_DIR}/Cell/DataManagement/JSON.hh
    ${MODULE_INCLUDES_DIR}/Cell/DataManagement/Model.hh
    ${MODULE_INCLUDES_DIR}/Cell/DataManagement/Result.hh
    ${MODULE_INCLUDES_DIR}/Cell/DataManagement/Texture.hh
    ${MODULE_INCLUDES_DIR}/Cell/DataManagement/zlib.hh

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Archive.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Base64.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Checksum.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/JSON.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/zlib.cc

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Model/FromGLTF.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Model/Model.cc

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Texture/FromPNG.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Texture/Texture.cc

    ${CMAKE_CURRENT_LIST_DIR}/Sources/HTTP/Connection.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/HTTP/Request.cc
)

target_compile_definitions(CellDataManagement PUBLIC "CELL_MODULES_DATAMANAGEMENT_AVAILABLE=1")
target_include_directories(CellDataManagement PUBLIC ${MODULE_INCLUDES_DIR})
target_link_libraries(CellDataManagement PUBLIC CellCore)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/ThirdParty/zlib)
target_link_libraries(CellDataManagement PRIVATE zlib)

if (CELL_ENABLE_TESTS AND CELL_ENABLE_TESTS_LIKELY_FAILURE)
    function(cell_dm_add_test name)
        add_executable(CellDataManagementParsing${name}Test ${CMAKE_CURRENT_LIST_DIR}/Tests/${name}.cc)
        target_link_libraries(CellDataManagementParsing${name}Test PRIVATE CellDataManagement)
        add_test(NAME CellDataManagementParsing${name}Test COMMAND CellDataManagementParsing${name}Test WORKING_DIRECTORY ${PROJECT_ROOT_DIR})
        cell_target_bootstrap(CellDataManagementParsing${name}Test TRUE)
    endfunction()

    cell_dm_add_test(glTF)
    cell_dm_add_test(HTTP)
    cell_dm_add_test(JSON)
    cell_dm_add_test(PNG)
endif ()

cell_target_bootstrap(CellDataManagement)

if (CELL_ENABLE_UTILITIES)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Utilities/ContentPacker)
endif ()
