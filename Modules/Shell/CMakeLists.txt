# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(CellShell LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Cell - Modules - Shell")

set(MODULE_INCLUDES_DIR ${CMAKE_CURRENT_LIST_DIR}/Includes)

add_library(CellShell ${CELL_LIBRARY_TYPE}
    ${MODULE_INCLUDES_DIR}/Cell/Shell/Controller.hh
    ${MODULE_INCLUDES_DIR}/Cell/Shell/InputTypes.hh
    ${MODULE_INCLUDES_DIR}/Cell/Shell/Result.hh
    ${MODULE_INCLUDES_DIR}/Cell/Shell/Shell.hh

    ${MODULE_INCLUDES_DIR}/Cell/Shell/Controller/DualSense.hh
    ${MODULE_INCLUDES_DIR}/Cell/Shell/Controller/DualShock4.hh
    ${MODULE_INCLUDES_DIR}/Cell/Shell/Controller/SwitchPro.hh
    ${MODULE_INCLUDES_DIR}/Cell/Shell/Controller/Xbox.hh

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Actions.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Controller.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/RunDispatch.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Shell.cc

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Controller/DualSenseStructures.hh
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Controller/DualSense.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Controller/DualShock4.cc

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Controller/SwitchPro/Structures.hh
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Controller/SwitchPro/Commands.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Controller/SwitchPro/SwitchPro.cc
)

target_compile_definitions(CellShell PUBLIC "CELL_MODULES_SHELL_AVAILABLE=1")
target_compile_options(CellShell PRIVATE -Wno-nested-anon-types)
target_include_directories(CellShell PUBLIC ${MODULE_INCLUDES_DIR})
target_link_libraries(CellShell PUBLIC CellCore)

if (WIN32)
    target_sources(CellShell PRIVATE
        ${MODULE_INCLUDES_DIR}/Cell/Shell/Implementations/Windows.hh

        ${CMAKE_CURRENT_LIST_DIR}/Sources/Windows/Manifest.rc.h

        ${CMAKE_CURRENT_LIST_DIR}/Sources/Windows/Controller/Xbox.cc

        ${CMAKE_CURRENT_LIST_DIR}/Sources/Windows/KeyboardHandler.cc
        ${CMAKE_CURRENT_LIST_DIR}/Sources/Windows/NewDestruct.cc
        ${CMAKE_CURRENT_LIST_DIR}/Sources/Windows/WindowLoop.cc
        ${CMAKE_CURRENT_LIST_DIR}/Sources/Windows/Windows.cc
    )

    target_compile_options(CellShell PRIVATE -Wno-language-extension-token)
    target_link_libraries(CellShell PRIVATE dwmapi)
elseif (APPLE)
    target_sources(CellShell PRIVATE
        ${MODULE_INCLUDES_DIR}/Cell/Shell/Implementations/macOS.hh

        ${CMAKE_CURRENT_LIST_DIR}/Sources/macOS/Backend.hh

        ${CMAKE_CURRENT_LIST_DIR}/Sources/macOS/Backend.mm
        ${CMAKE_CURRENT_LIST_DIR}/Sources/macOS/macOS.mm
        ${CMAKE_CURRENT_LIST_DIR}/Sources/macOS/WindowImpl.mm

        ${CMAKE_CURRENT_LIST_DIR}/Sources/macOS/Controller/Xbox.cc
    )

    target_link_libraries(CellShell PRIVATE "-framework Cocoa" "-framework QuartzCore" c++)
elseif (UNIX)
    find_package(wayland REQUIRED FATAL_ERROR)

    target_sources(CellShell PRIVATE
        ${MODULE_INCLUDES_DIR}/Cell/Shell/Implementations/Linux.hh

        ${MODULE_INCLUDES_DIR}/Cell/Shell/Implementations/Linux/Protocols/cursor-shape-client-protocol.h
        ${MODULE_INCLUDES_DIR}/Cell/Shell/Implementations/Linux/Protocols/idle-inhibit-client-protocol.h
        ${MODULE_INCLUDES_DIR}/Cell/Shell/Implementations/Linux/Protocols/pointer-constraints-client-protocol.h
        ${MODULE_INCLUDES_DIR}/Cell/Shell/Implementations/Linux/Protocols/relative-pointer-client-protocol.h
        ${MODULE_INCLUDES_DIR}/Cell/Shell/Implementations/Linux/Protocols/tablet-client-protocol.h
        ${MODULE_INCLUDES_DIR}/Cell/Shell/Implementations/Linux/Protocols/xdg-decoration-client-protocol.h
        ${MODULE_INCLUDES_DIR}/Cell/Shell/Implementations/Linux/Protocols/xdg-shell-client-protocol.h

        ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Linux.cc
        ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/NewDestruct.cc

        ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Events/Events.cc
        ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Events/Keyboard.cc
        ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Events/Pointer.cc
        ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Events/Registry.cc
        ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Events/Seat.cc
        ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Events/XDGToplevel.cc

        ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Protocols/cursor-shape-protocol.c
        ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Protocols/idle-inhibit-protocol.c
        ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Protocols/pointer-constraints-protocol.c
        ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Protocols/relative-pointer-protocol.c
        ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Protocols/tablet-protocol.c
        ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Protocols/xdg-decoration-protocol.c
        ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Protocols/xdg-shell-protocol.c

        ${CMAKE_CURRENT_LIST_DIR}/Sources/Linux/Controller/Xbox.cc
    )

    target_include_directories(CellShell PRIVATE ${wayland_INCLUDE_DIRS} ${xkbcommon_INCLUDE_DIRS})
    target_link_libraries(CellShell PRIVATE ${wayland_LIBRARIES} ${xkbcommon_LIBRARIES})
else ()
    message(FATAL_ERROR "Unsupported platform.")
endif ()

cell_target_bootstrap(CellShell)
