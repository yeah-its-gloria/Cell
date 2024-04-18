# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

target_sources(CellCore PRIVATE
    ${PLATFORM_DIR}/Sources/VirtualStub.cc

    ${PLATFORM_DIR}/Sources/String.mm

    ${PLATFORM_DIR}/Sources/IO/File/File.mm
    ${PLATFORM_DIR}/Sources/IO/File/CheckPathSetWorkingDirectory.mm
    ${PLATFORM_DIR}/Sources/IO/File/OpenCreate.mm
    ${PLATFORM_DIR}/Sources/IO/File/ReadWriteFlush.mm

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
    ${PLATFORM_DIR}/Sources/System/Mutex.mm
    ${PLATFORM_DIR}/Sources/System/Panic.mm
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

target_link_libraries(CellCore PRIVATE dl m "-framework Foundation" "-framework AppKit" "-framework IOKit" c++)
target_link_options(CellCore PUBLIC -Wl,-no_warn_duplicate_libraries)

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

    set_target_properties(${target} PROPERTIES
        MACOSX_DEPLOYMENT_TARGET 10.13.0

        XCODE_GENERATE_SCHEME TRUE
    )

    if (is_tool)
        target_compile_definitions(${target} PRIVATE "CELL_SYSTEM_IS_TOOL=true")
        return()
    endif ()

    target_compile_definitions(${target} PRIVATE "CELL_SYSTEM_IS_TOOL=false")

    target_sources(${target} PRIVATE ${PLATFORM_DIR}/BuildContent/Cell.icns)
    set_source_files_properties(${PLATFORM_DIR}/BuildContent/Cell.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

    set_target_properties(${target} PROPERTIES
        MACOSX_BUNDLE True
        MACOSX_BUNDLE_INFO_PLIST ${PLATFORM_DIR}/BuildContent/Info.plist.in
        MACOSX_BUNDLE_EXECUTABLE_NAME ${target}

        XCODE_SCHEME_WORKING_DIRECTORY ${PROJECT_ROOT_DIR}
    )
endfunction()
