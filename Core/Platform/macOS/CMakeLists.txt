# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

enable_language(OBJC)

target_sources(CellCore PRIVATE
    ${PLATFORM_DIR}/Sources/VirtualStub.cc

    ${PLATFORM_DIR}/Sources/IO/File/File.cc
    ${PLATFORM_DIR}/Sources/IO/File/CheckPath.cc
    ${PLATFORM_DIR}/Sources/IO/File/Delete.cc
    ${PLATFORM_DIR}/Sources/IO/File/Offset.cc
    ${PLATFORM_DIR}/Sources/IO/File/Open.cc
    ${PLATFORM_DIR}/Sources/IO/File/ReadWrite.cc
    ${PLATFORM_DIR}/Sources/IO/File/SetWorkingDirectory.cc

    ${PLATFORM_DIR}/Sources/IO/Directory.cc
    ${PLATFORM_DIR}/Sources/IO/HID.cc
    ${PLATFORM_DIR}/Sources/IO/Pipe.cc
    ${PLATFORM_DIR}/Sources/IO/USB.cc

    ${PLATFORM_DIR}/Sources/Memory/Allocator.cc

    ${PLATFORM_DIR}/Sources/Network/Internal.hh
    ${PLATFORM_DIR}/Sources/Network/AddressInfo.cc
    ${PLATFORM_DIR}/Sources/Network/TypeConversion.cc
    ${PLATFORM_DIR}/Sources/Network/Socket/NewDestruct.cc
    ${PLATFORM_DIR}/Sources/Network/Socket/Socket.cc

    ${PLATFORM_DIR}/Sources/System/DynamicLibrary.cc
    ${PLATFORM_DIR}/Sources/System/Event.cc
    ${PLATFORM_DIR}/Sources/System/Log.cc
    ${PLATFORM_DIR}/Sources/System/Mutex.cc
    ${PLATFORM_DIR}/Sources/System/Panic.cc
    ${PLATFORM_DIR}/Sources/System/String.cc
    ${PLATFORM_DIR}/Sources/System/Thread.mm
    ${PLATFORM_DIR}/Sources/System/Timer.cc
)

target_compile_definitions(CellCore PUBLIC
    "CELL_PLATFORM_MACOS=1"

    "$<$<CONFIG:Debug>:_DEBUG>"
)

if (CELL_LIBRARY_TYPE STREQUAL "STATIC")
    target_compile_definitions(CellCore PUBLIC "CELL_FUNCTION=")
elseif (CELL_LIBRARY_TYPE STREQUAL "SHARED")
    target_compile_definitions(CellCore PUBLIC "CELL_FUNCTION=__attribute__((visibility(\"default\")))")
else ()
    message(FATAL_ERROR "\"${CELL_LIBRARY_TYPE}\" is an invalid library type.")
endif ()

target_link_libraries(CellCore PUBLIC dl m "-framework Foundation" "-framework AppKit" PRIVATE c++)

function(cell_target_bootstrap target)
    get_target_property(type ${target} TYPE)
    if (NOT "${type}" STREQUAL "EXECUTABLE")
        return()
    endif ()

    if (${ARGC} GREATER 1)
        set(is_tool ${ARGV1})
    else ()
        set(is_tool FALSE)
    endif ()

    target_sources(${target} PRIVATE ${PLATFORM_DIR}/Sources/Bootstrap.mm)

    if (is_tool)
        target_compile_definitions(${target} PRIVATE "CELL_SYSTEM_IS_TOOL=true")
    else ()
        target_compile_definitions(${target} PRIVATE "CELL_SYSTEM_IS_TOOL=false")
    endif ()
endfunction()
