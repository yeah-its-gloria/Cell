# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(CellJavaScript LANGUAGES C CXX VERSION 1.0.0 DESCRIPTION "Cell - Modules - JavaScript Engine")

set(MODULE_INCLUDES_DIR ${CMAKE_CURRENT_LIST_DIR}/Includes)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/ThirdParty/QuickJS)

add_library(CellJavaScript ${CELL_LIBRARY_TYPE}
    ${MODULE_INCLUDES_DIR}/Cell/JavaScript/Engine.hh

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Engine.cc
)

target_compile_definitions(CellJavaScript PUBLIC "CELL_MODULES_JAVASCRIPT_AVAILABLE=1")
target_include_directories(CellJavaScript PUBLIC ${MODULE_INCLUDES_DIR} ${CMAKE_CURRENT_LIST_DIR}/ThirdParty/QuickJS)
target_link_libraries(CellJavaScript PUBLIC CellCore QuickJS)

if (NOT WIN32 AND NOT UNIX)
    message(FATAL_ERROR "Unsupported platform.")
endif ()

cell_target_bootstrap(CellJavaScript)

if (CELL_ENABLE_TESTS AND CELL_ENABLE_TESTS_LIKELY_FAILURE)
    add_executable(CellJavaScriptGenericTest ${CMAKE_CURRENT_LIST_DIR}/Tests/Generic.cc)
    target_link_libraries(CellJavaScriptGenericTest PRIVATE CellJavaScript)
    add_test(NAME CellJavaScriptGenericTest COMMAND CellJavaScriptGenericTest WORKING_DIRECTORY ${PROJECT_ROOT_DIR})
    cell_target_bootstrap(CellJavaScriptGenericTest TRUE)
endif ()
