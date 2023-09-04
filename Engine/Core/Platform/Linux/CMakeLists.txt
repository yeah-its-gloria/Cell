# SPDX-FileCopyrightText: Copyright 2023 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

find_package(wayland REQUIRED FATAL_ERROR)
find_package(xkbcommon REQUIRED FATAL_ERROR)

target_sources(CellCore PRIVATE
               ${PLATFORM_DIR}/Sources/VirtualStub.cc

               ${PLATFORM_DIR}/Sources/IO/File/File.cc
               ${PLATFORM_DIR}/Sources/IO/File/CheckPath.cc
               ${PLATFORM_DIR}/Sources/IO/File/Delete.cc
               ${PLATFORM_DIR}/Sources/IO/File/Open.cc
               ${PLATFORM_DIR}/Sources/IO/File/SetWorkingDirectory.cc

               ${PLATFORM_DIR}/Sources/IO/FolderWalker.cc
               ${PLATFORM_DIR}/Sources/IO/Pipe.cc
               ${PLATFORM_DIR}/Sources/IO/USB.cc

               ${PLATFORM_DIR}/Sources/Network/Internal.hh
               ${PLATFORM_DIR}/Sources/Network/AddressInfo.cc
               ${PLATFORM_DIR}/Sources/Network/Socket/DestructAndNew.cc
               ${PLATFORM_DIR}/Sources/Network/Socket/Socket.cc

               ${PLATFORM_DIR}/Sources/Shell/DrawableExtentForWindow.cc
               ${PLATFORM_DIR}/Sources/Shell/Input.cc
               ${PLATFORM_DIR}/Sources/Shell/SetNewTitleForWindow.cc

               ${PLATFORM_DIR}/Sources/System/DynamicLibrary.cc
               ${PLATFORM_DIR}/Sources/System/Event.cc
               ${PLATFORM_DIR}/Sources/System/Log.cc
               ${PLATFORM_DIR}/Sources/System/Memory.cc
               ${PLATFORM_DIR}/Sources/System/Mutex.cc
               ${PLATFORM_DIR}/Sources/System/Panic.cc
               ${PLATFORM_DIR}/Sources/System/Sleep.cc
               ${PLATFORM_DIR}/Sources/System/String.cc
               ${PLATFORM_DIR}/Sources/System/Thread.cc
               ${PLATFORM_DIR}/Sources/System/Ticker.cc

               ${PLATFORM_DIR}/Sources/System/Platform/Linux.cc
               ${PLATFORM_DIR}/Sources/System/Platform/Wayland.cc
               ${PLATFORM_DIR}/Sources/System/Platform/WaylandRegistry.cc
               ${PLATFORM_DIR}/Sources/System/Platform/WaylandKeyboard.cc
               ${PLATFORM_DIR}/Sources/System/Platform/WaylandSeat.cc
               ${PLATFORM_DIR}/Sources/System/Platform/WaylandXDGToplevel.cc

               ${PLATFORM_DIR}/Sources/System/Platform/WaylandProtocols/idle-inhibit-protocol.c
               ${PLATFORM_DIR}/Sources/System/Platform/WaylandProtocols/xdg-decoration-protocol.c
               ${PLATFORM_DIR}/Sources/System/Platform/WaylandProtocols/xdg-shell-protocol.c
)

target_compile_definitions(CellCore PUBLIC
                           "CELL_PLATFORM_LINUX=1"

                           "_FILE_OFFSET_BITS=64"
                           "_REENTRANT"
)

target_compile_options(CellCore PRIVATE -Wno-varargs)

target_link_options(CellCore PUBLIC -Wl,--no-undefined)

if (NOT DEFINED ${CMAKE_BUILD_TYPE} OR ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_compile_definitions(CellCore PUBLIC "_DEBUG")
endif ()

if (CELL_LIBRARY_TYPE STREQUAL "STATIC")
    target_compile_definitions(CellCore PUBLIC "CELL_FUNCTION=")
elseif (CELL_LIBRARY_TYPE STREQUAL "SHARED")
    target_compile_definitions(CellCore PUBLIC "CELL_FUNCTION=__attribute__((visibility(\"default\")))")
else ()
    message("${CELL_LIBRARY_TYPE} is an invalid library type")
endif ()

target_include_directories(CellCore PUBLIC ${wayland_INCLUDE_DIRS} ${xkbcommon_INCLUDE_DIRS})

target_link_libraries(CellCore PRIVATE
                      ${wayland_LIBRARIES}
                      ${xkbcommon_LIBRARIES}

                      PUBLIC pthread
                      PUBLIC dl
                      PUBLIC rt
                      PUBLIC m
)

function(Cell_Platform_Bootstrap_Target target)
    get_target_property(type ${target} TYPE)
    if (NOT "${type}" STREQUAL "EXECUTABLE" AND NOT "${type}" STREQUAL "SHARED_LIBRARY")
        return()
    endif ()

    if (${ARGC} GREATER 1)
        set(is_tool ${ARGV1})
    else ()
        set(is_tool FALSE)
    endif ()

    if ("${type}" STREQUAL "EXECUTABLE")
        target_sources(${target} PRIVATE ${PLATFORM_DIR}/Sources/Bootstrap.cc)

        if (is_tool)
            target_compile_definitions(${target} PRIVATE "CELL_SYSTEM_IS_TOOL=true")
        else ()
            target_compile_definitions(${target} PRIVATE "CELL_SYSTEM_IS_TOOL=false")
        endif ()
    endif ()
endfunction()
