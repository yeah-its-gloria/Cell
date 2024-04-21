# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

if (NOT APPLE)
    message(FATAL_ERROR "Unsupported platform.")
endif ()

project(CellMetal LANGUAGES CXX OBJC VERSION 1.0.0 DESCRIPTION "Cell - Modules - Metal")

set(MODULE_INCLUDES_DIR ${CMAKE_CURRENT_LIST_DIR}/Includes)

add_library(CellMetal ${CELL_LIBRARY_TYPE}
    ${MODULE_INCLUDES_DIR}/Cell/Metal/Device.hh

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Device.mm
)

target_compile_definitions(CellMetal PUBLIC "CELL_MODULES_METAL_AVAILABLE=1")
target_compile_options(CellMetal PRIVATE -Wno-language-extension-token)
target_include_directories(CellMetal PUBLIC ${MODULE_INCLUDES_DIR})
target_link_libraries(CellMetal
    PUBLIC CellCore
    PUBLIC CellShell
    PRIVATE "-framework CoreFoundation"
    PRIVATE "-framework CoreGraphics"
    PRIVATE "-framework Metal"
    PRIVATE c++
)

cell_target_bootstrap(CellMetal)

add_executable(CellMetalRunner
    ${CMAKE_CURRENT_LIST_DIR}/Utilities/Runner.cc
)

target_link_libraries(CellMetalRunner PRIVATE CellMetal)
cell_target_bootstrap(CellMetalRunner TRUE)
