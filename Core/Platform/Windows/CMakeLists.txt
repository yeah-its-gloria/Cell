# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

option(CELL_PLATFORM_WINDOWS_LOG_TO_CONSOLE "Whether to log to the console or via OutputDebugStringW" OFF)

find_file(CLANG_RT_BUILTINS NAME "clang_rt.builtins-x86_64.lib" REQUIRED HINTS
    "C:\\Program Files\\LLVM\\lib\\clang\\16\\lib\\windows\\"
    "C:\\Program Files\\LLVM\\lib\\clang\\17\\lib\\windows\\"
)

target_sources(CellCore PRIVATE
    ${PLATFORM_DIR}/Includes/Cell/System/Platform/Windows/Includes.h

    ${PLATFORM_DIR}/Sources/String.cc

    ${PLATFORM_DIR}/Sources/IO/File/File.cc
    ${PLATFORM_DIR}/Sources/IO/File/CheckPath.cc
    ${PLATFORM_DIR}/Sources/IO/File/Delete.cc
    ${PLATFORM_DIR}/Sources/IO/File/Open.cc
    ${PLATFORM_DIR}/Sources/IO/File/ReadWrite.cc
    ${PLATFORM_DIR}/Sources/IO/File/SetWorkingDirectory.cc

    ${PLATFORM_DIR}/Sources/IO/Directory/Directory.cc

    ${PLATFORM_DIR}/Sources/IO/HID/HID.cc
    ${PLATFORM_DIR}/Sources/IO/HID/Open.cc

    ${PLATFORM_DIR}/Sources/IO/Pipe/Pipe.cc
    ${PLATFORM_DIR}/Sources/IO/Pipe/ClientManagement.cc
    ${PLATFORM_DIR}/Sources/IO/Pipe/Connect.cc
    ${PLATFORM_DIR}/Sources/IO/Pipe/Create.cc
    ${PLATFORM_DIR}/Sources/IO/Pipe/WaitUntilReady.cc

    ${PLATFORM_DIR}/Sources/IO/USB/Internal.hh
    ${PLATFORM_DIR}/Sources/IO/USB/USB.cc
    ${PLATFORM_DIR}/Sources/IO/USB/Open.cc

    ${PLATFORM_DIR}/Sources/Memory/Allocator.cc

    ${PLATFORM_DIR}/Sources/Network/Internal.hh
    ${PLATFORM_DIR}/Sources/Network/AddressInfo.cc
    ${PLATFORM_DIR}/Sources/Network/TypeConversion.cc
    ${PLATFORM_DIR}/Sources/Network/Socket/Communication.cc
    ${PLATFORM_DIR}/Sources/Network/Socket/Connection.cc
    ${PLATFORM_DIR}/Sources/Network/Socket/NewDestruct.cc

    ${PLATFORM_DIR}/Sources/System/DynamicLibrary.cc
    ${PLATFORM_DIR}/Sources/System/Event.cc
    ${PLATFORM_DIR}/Sources/System/Log.cc
    ${PLATFORM_DIR}/Sources/System/Mutex.cc
    ${PLATFORM_DIR}/Sources/System/Panic.cc
    ${PLATFORM_DIR}/Sources/System/RNG.cc
    ${PLATFORM_DIR}/Sources/System/Thread.cc
    ${PLATFORM_DIR}/Sources/System/Timer.cc
)

target_compile_definitions(CellCore PUBLIC
    "CELL_PLATFORM_WINDOWS=1"

    "_CRT_SECURE_NO_WARNINGS=1"
)

if (CELL_PLATFORM_WINDOWS_LOG_TO_CONSOLE)
    target_compile_definitions(CellCore PRIVATE "CELL_PLATFORM_WINDOWS_LOG_TO_CONSOLE=1")
endif ()

if (CELL_LIBRARY_TYPE STREQUAL "STATIC")
    target_compile_definitions(CellCore PUBLIC "CELL_FUNCTION=")
elseif (CELL_LIBRARY_TYPE STREQUAL "SHARED")
    target_compile_definitions(CellCore PUBLIC "CELL_FUNCTION=__declspec(dllexport)")
else ()
    message(FATAL_ERROR "\"${CELL_LIBRARY_TYPE}\" is an invalid library type.")
endif ()

target_compile_options(CellCore PRIVATE -Wno-varargs)

target_include_directories(CellCore PUBLIC ${PLATFORM_DIR}/Includes/)

target_link_libraries(CellCore PRIVATE
    bcrypt
    hid
    ntdll
    setupapi
    shlwapi
    winusb

    PUBLIC kernel32
    PUBLIC runtimeobject
    PUBLIC userenv
    PUBLIC user32
    PUBLIC ws2_32
    PUBLIC ${CLANG_RT_BUILTINS}
)

target_link_options(CellCore PUBLIC -fuse-ld=lld)

function(Cell_Platform_Windows_Generate_Manifests target type)
    get_target_property(NAME ${target} NAME)

    set(DESCRIPTION ${PROJECT_DESCRIPTION})
    set(VMAJOR ${PROJECT_VERSION_MAJOR})
    set(VMINOR ${PROJECT_VERSION_MINOR})
    set(VPATCH ${PROJECT_VERSION_PATCH})

    if ("${type}" STREQUAL "EXECUTABLE")
        set(FILENAME "${NAME}.exe")
        set(FILETYPE "VFT_APP")
    else ()
        set(FILENAME "${NAME}.dll")
        set(FILETYPE "VFT_DLL")
    endif ()

    configure_file(${PLATFORM_DIR}/BuildContent/Manifest.manifest.in ${GENERATE_DIR}/${target}.manifest)
    configure_file(${PLATFORM_DIR}/BuildContent/Manifest.rc.in ${GENERATE_DIR}/${target}.rc)

    target_sources(${target} PRIVATE
        ${GENERATE_DIR}/${target}.manifest
        ${GENERATE_DIR}/${target}.rc
    )
endfunction()

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

    target_sources(${target} PRIVATE ${PLATFORM_DIR}/Sources/Bootstrap.cc)

    if ("${type}" STREQUAL "EXECUTABLE")
        if (is_tool)
            target_compile_definitions(${target} PRIVATE "CELL_SYSTEM_IS_TOOL=true")
            set(subsystem "console")
        else ()
            target_compile_definitions(${target} PRIVATE "CELL_SYSTEM_IS_TOOL=false")
            set(subsystem "windows")
        endif ()

        if (MSVC) # MSVC linking flags are passed directly to LINK, clang intercepts anything sent to lld first
            set_target_properties(${target} PROPERTIES LINK_FLAGS "-subsystem:${subsystem} -entry:mainCRTStartup")
        else ()
            set_target_properties(${target} PROPERTIES LINK_FLAGS "-Wl,-subsystem:${subsystem} -Wl,-entry:mainCRTStartup")
        endif ()

        target_link_libraries(${target} PRIVATE ntdll)
    endif ()

    Cell_Platform_Windows_Generate_Manifests(${target} ${type})
endfunction()
