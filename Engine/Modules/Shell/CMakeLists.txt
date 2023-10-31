# SPDX-FileCopyrightText: Copyright 2023 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(CellShell LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Cell - Modules - Shell")

set(MODULE_INCLUDES_DIR ${CMAKE_CURRENT_LIST_DIR}/Includes)

add_library(CellShell ${CELL_LIBRARY_TYPE}
    ${MODULE_INCLUDES_DIR}/Cell/Shell/Controller.hh
    ${MODULE_INCLUDES_DIR}/Cell/Shell/Input.hh
    ${MODULE_INCLUDES_DIR}/Cell/Shell/InputTypes.hh
    ${MODULE_INCLUDES_DIR}/Cell/Shell/Result.hh
    ${MODULE_INCLUDES_DIR}/Cell/Shell/Shell.hh

    ${MODULE_INCLUDES_DIR}/Cell/Shell/Controller/DualSense.hh
    ${MODULE_INCLUDES_DIR}/Cell/Shell/Controller/Xbox.hh

    ${MODULE_INCLUDES_DIR}/Cell/Shell/Implementations/Windows.hh

    ${MODULE_INCLUDES_DIR}/Cell/Shell/Implementations/Linux.hh
    ${MODULE_INCLUDES_DIR}/Cell/Shell/Implementations/Linux/WaylandProtocols/idle-inhibit-client-protocol.h
    ${MODULE_INCLUDES_DIR}/Cell/Shell/Implementations/Linux/WaylandProtocols/xdg-decoration-client-protocol.h
    ${MODULE_INCLUDES_DIR}/Cell/Shell/Implementations/Linux/WaylandProtocols/xdg-shell-client-protocol.h

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Shell.cc

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Input/Actions.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Input/Controller.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Input/Poll.cc

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Controller/DualSense.cc
)

target_compile_definitions(CellShell PUBLIC "CELL_MODULES_SHELL_AVAILABLE=1")
target_include_directories(CellShell PUBLIC ${MODULE_INCLUDES_DIR})
target_link_libraries(CellShell PUBLIC CellCore)

if (WIN32)
    target_sources(CellShell PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Sources/Windows/DrawableExtentForWindow.cc
        ${CMAKE_CURRENT_LIST_DIR}/Sources/Windows/SetNewTitleForWindow.cc

        ${CMAKE_CURRENT_LIST_DIR}/Sources/Windows/Shell/Manifest.rc.h

        ${CMAKE_CURRENT_LIST_DIR}/Sources/Windows/Controller/Xbox.cc

        ${CMAKE_CURRENT_LIST_DIR}/Sources/Windows/Shell/KeyboardHandler.cc
        ${CMAKE_CURRENT_LIST_DIR}/Sources/Windows/Shell/WindowLoop.cc
        ${CMAKE_CURRENT_LIST_DIR}/Sources/Windows/Shell/Windows.cc
    )

    target_compile_options(CellShell PRIVATE -Wno-language-extension-token)
    target_link_libraries(CellShell PRIVATE dwmapi)
elseif (UNIX)
    find_package(wayland REQUIRED FATAL_ERROR)

    target_sources(CellShell PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/DrawableExtent.cc
        ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/SetNewTitle.cc

        ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Shell/WaylandProtocols/idle-inhibit-protocol.c
        ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Shell/WaylandProtocols/xdg-decoration-protocol.c
        ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Shell/WaylandProtocols/xdg-shell-protocol.c

        ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Controller/Xbox.cc

        ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Shell/Linux.cc
        ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Shell/Wayland.cc
        ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Shell/WaylandKeyboard.cc
        ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Shell/WaylandRegistry.cc
        ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Shell/WaylandSeat.cc
        ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Shell/WaylandXDGToplevel.cc
    )

    target_include_directories(CellShell PRIVATE ${wayland_INCLUDE_DIRS} ${xkbcommon_INCLUDE_DIRS})
    target_link_libraries(CellShell PRIVATE ${wayland_LIBRARIES} ${xkbcommon_LIBRARIES})
else ()
    message(FATAL_ERROR "Unsupported platform")
endif ()

CellPlatformBootstrapTarget(CellShell)
