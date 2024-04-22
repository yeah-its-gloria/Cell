# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

set(SUBMODULE_INCLUDES_DIR ${CMAKE_CURRENT_LIST_DIR}/Includes)

target_sources(CellRenderer PRIVATE
    ${SUBMODULE_INCLUDES_DIR}/Cell/Renderer/Metal/Device.hh

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Device.mm
)

target_compile_definitions(CellRenderer PUBLIC "CELL_MODULES_RENDERER_METAL_AVAILABLE=1")
target_include_directories(CellRenderer PUBLIC ${SUBMODULE_INCLUDES_DIR})
target_link_libraries(CellRenderer PRIVATE
    "-framework CoreFoundation"
    "-framework CoreGraphics"
    "-framework Metal"
    c++
)

add_executable(CellRendererMetalRunner
    ${CMAKE_CURRENT_LIST_DIR}/Utilities/Runner.cc
)

target_link_libraries(CellRendererMetalRunner PRIVATE CellRenderer)
cell_target_bootstrap(CellRendererMetalRunner TRUE)
