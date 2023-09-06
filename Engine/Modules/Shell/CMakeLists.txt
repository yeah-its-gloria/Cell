# SPDX-FileCopyrightText: Copyright 2023 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(CellModulesShell LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Cell - Modules - Shell")

set(MODULE_INCLUDES_DIR ${CMAKE_CURRENT_LIST_DIR}/Includes)

add_library(CellModulesShell ${CELL_LIBRARY_TYPE}
            ${MODULE_INCLUDES_DIR}/Cell/Shell/Input.hh
            ${MODULE_INCLUDES_DIR}/Cell/Shell/KeyValue.hh
            ${MODULE_INCLUDES_DIR}/Cell/Shell/Result.hh
            ${MODULE_INCLUDES_DIR}/Cell/Shell/Shell.hh

            ${MODULE_INCLUDES_DIR}/Cell/Shell/Implementations/Windows.hh

            ${MODULE_INCLUDES_DIR}/Cell/Shell/Implementations/Linux.hh
            ${MODULE_INCLUDES_DIR}/Cell/Shell/Implementations/Linux/WaylandProtocols/idle-inhibit-client-protocol.h
            ${MODULE_INCLUDES_DIR}/Cell/Shell/Implementations/Linux/WaylandProtocols/xdg-decoration-client-protocol.h
            ${MODULE_INCLUDES_DIR}/Cell/Shell/Implementations/Linux/WaylandProtocols/xdg-shell-client-protocol.h

            ${CMAKE_CURRENT_LIST_DIR}/Sources/Input.cc
            ${CMAKE_CURRENT_LIST_DIR}/Sources/Shell.cc
)

target_compile_definitions(CellModulesShell PUBLIC "CELL_MODULES_SHELL_AVAILABLE=1")
target_include_directories(CellModulesShell PUBLIC ${MODULE_INCLUDES_DIR})
target_link_libraries(CellModulesShell PUBLIC CellCore)

if (WIN32)
    target_sources(CellModulesShell PRIVATE
                   ${CMAKE_CURRENT_LIST_DIR}/Sources/Windows/DrawableExtentForWindow.cc
                   ${CMAKE_CURRENT_LIST_DIR}/Sources/Windows/Input.cc
                   ${CMAKE_CURRENT_LIST_DIR}/Sources/Windows/SetNewTitleForWindow.cc

                   ${CMAKE_CURRENT_LIST_DIR}/Sources/Windows/Shell/Manifest.rc.h

                   ${CMAKE_CURRENT_LIST_DIR}/Sources/Windows/Shell/KeyboardHandler.cc
                   ${CMAKE_CURRENT_LIST_DIR}/Sources/Windows/Shell/WindowLoop.cc
                   ${CMAKE_CURRENT_LIST_DIR}/Sources/Windows/Shell/Windows.cc
    )

    target_link_libraries(CellModulesShell PRIVATE dwmapi)
elseif (UNIX)
    target_sources(CellModulesShell PRIVATE
                   ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/DrawableExtentForWindow.cc
                   ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Input.cc
                   ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/SetNewTitleForWindow.cc

                   ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Shell/WaylandProtocols/idle-inhibit-protocol.c
                   ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Shell/WaylandProtocols/xdg-decoration-protocol.c
                   ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Shell/WaylandProtocols/xdg-shell-protocol.c

                   ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Shell/Linux.cc
                   ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Shell/Wayland.cc
                   ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Shell/WaylandKeyboard.cc
                   ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Shell/WaylandRegistry.cc
                   ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Shell/WaylandSeat.cc
                   ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Shell/WaylandXDGToplevel.cc
    )
else ()
    message(FATAL_ERROR "Unsupported platform")
endif ()

Cell_Platform_Bootstrap_Target(CellModulesShell)
