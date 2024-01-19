# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

target_sources(CellCore PRIVATE
               ${PLATFORM_DIR}/Sources/VirtualStub.cc

               ${PLATFORM_DIR}/Sources/IO/File/File.cc
               ${PLATFORM_DIR}/Sources/IO/File/CheckPath.cc
               ${PLATFORM_DIR}/Sources/IO/File/Delete.cc
               ${PLATFORM_DIR}/Sources/IO/File/Offset.cc
               ${PLATFORM_DIR}/Sources/IO/File/Open.cc
               ${PLATFORM_DIR}/Sources/IO/File/ReadWrite.cc
               ${PLATFORM_DIR}/Sources/IO/File/SetWorkingDirectory.cc

               ${PLATFORM_DIR}/Sources/IO/FolderWalker.cc
               ${PLATFORM_DIR}/Sources/IO/HID.cc
               ${PLATFORM_DIR}/Sources/IO/Pipe.cc
               ${PLATFORM_DIR}/Sources/IO/USB.cc

               ${PLATFORM_DIR}/Sources/Network/Internal.hh
               ${PLATFORM_DIR}/Sources/Network/AddressInfo.cc
               ${PLATFORM_DIR}/Sources/Network/Socket/NewDestruct.cc
               ${PLATFORM_DIR}/Sources/Network/Socket/Socket.cc

               ${PLATFORM_DIR}/Sources/System/DynamicLibrary.cc
               ${PLATFORM_DIR}/Sources/System/Event.cc
               ${PLATFORM_DIR}/Sources/System/Log.cc
               ${PLATFORM_DIR}/Sources/System/Memory.cc
               ${PLATFORM_DIR}/Sources/System/Mutex.cc
               ${PLATFORM_DIR}/Sources/System/Panic.cc
               ${PLATFORM_DIR}/Sources/System/String.cc
               ${PLATFORM_DIR}/Sources/System/Thread.cc
               ${PLATFORM_DIR}/Sources/System/Timer.cc
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

target_link_libraries(CellCore PUBLIC pthread dl rt m)

function(CellPlatformBootstrapTarget target)
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
