# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(CellCore LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Cell - Core")

option(CELL_CORE_SKIP_ASSERT "Whether to skip assert checks and simply continue. Defaults to OFF." OFF)
option(CELL_CORE_USE_PANIC_ASSERT "Whether to use a built in engine assert implementation. Calls Cell::System::Panic. Defaults to ON." ON)
option(CELL_CORE_USE_EXTERNAL_ASSERT "Whether to use assert from the C standard library implementation. Defaults to OFF." OFF)

configure_file(${CORE_DIR}/GeneratedIncludes/Cell/Version.h.in ${GENERATE_DIR}/Includes/Cell/Version.h)

add_library(CellCore ${CELL_LIBRARY_TYPE}
    ${CORE_INCLUDES_DIR}/Cell/Cell.hh

    ${CORE_INCLUDES_DIR}/Cell/Optional.hh
    ${CORE_INCLUDES_DIR}/Cell/Reference.hh
    ${CORE_INCLUDES_DIR}/Cell/Scoped.hh
    ${CORE_INCLUDES_DIR}/Cell/Wrapped.hh

    ${CORE_INCLUDES_DIR}/Cell/Collection/Dictionary.hh
    ${CORE_INCLUDES_DIR}/Cell/Collection/Enumerable.hh
    ${CORE_INCLUDES_DIR}/Cell/Collection/List.hh

    ${CORE_INCLUDES_DIR}/Cell/IO/File.hh
    ${CORE_INCLUDES_DIR}/Cell/IO/FolderWalker.hh
    ${CORE_INCLUDES_DIR}/Cell/IO/HID.hh
    ${CORE_INCLUDES_DIR}/Cell/IO/Pipe.hh
    ${CORE_INCLUDES_DIR}/Cell/IO/Result.hh
    ${CORE_INCLUDES_DIR}/Cell/IO/USB.hh

    ${CORE_INCLUDES_DIR}/Cell/Network/AddressInfo.hh
    ${CORE_INCLUDES_DIR}/Cell/Network/Properties.hh
    ${CORE_INCLUDES_DIR}/Cell/Network/Result.hh
    ${CORE_INCLUDES_DIR}/Cell/Network/Socket.hh

    ${CORE_INCLUDES_DIR}/Cell/System/Block.hh
    ${CORE_INCLUDES_DIR}/Cell/System/BlockImpl.hh
    ${CORE_INCLUDES_DIR}/Cell/System/Channel.hh
    ${CORE_INCLUDES_DIR}/Cell/System/DynamicLibrary.hh
    ${CORE_INCLUDES_DIR}/Cell/System/Entry.hh
    ${CORE_INCLUDES_DIR}/Cell/System/Event.hh
    ${CORE_INCLUDES_DIR}/Cell/System/Log.hh
    ${CORE_INCLUDES_DIR}/Cell/System/Memory.hh
    ${CORE_INCLUDES_DIR}/Cell/System/Mutex.hh
    ${CORE_INCLUDES_DIR}/Cell/System/Panic.hh
    ${CORE_INCLUDES_DIR}/Cell/System/Result.hh
    ${CORE_INCLUDES_DIR}/Cell/System/RNG.hh
    ${CORE_INCLUDES_DIR}/Cell/System/String.hh
    ${CORE_INCLUDES_DIR}/Cell/System/StringFormatting.hh
    ${CORE_INCLUDES_DIR}/Cell/System/Thread.hh
    ${CORE_INCLUDES_DIR}/Cell/System/Timer.hh

    ${CORE_INCLUDES_DIR}/Cell/Utilities/Byteswap.hh
    ${CORE_INCLUDES_DIR}/Cell/Utilities/MinMaxClamp.hh
    ${CORE_INCLUDES_DIR}/Cell/Utilities/Preprocessor.hh
    ${CORE_INCLUDES_DIR}/Cell/Utilities/RawString.hh

    ${GENERATE_DIR}/Includes/Cell/Version.h

    ${CORE_DIR}/Sources/Cell.cc
    ${CORE_DIR}/Sources/System/Panic.cc
    ${CORE_DIR}/Sources/System/String.cc
    ${CORE_DIR}/Sources/System/StringFormat.cc
)

target_compile_definitions(CellCore PUBLIC
                           "CELL_DEPRECATED=[[deprecated]]"
                           "CELL_NODISCARD=[[nodiscard]]"
                           "CELL_NORETURN=[[noreturn]]"
                           "CELL_UNMANGLED=extern \"C\""
)

if (CELL_CORE_SKIP_ASSERT)
    if (CELL_CORE_USE_EXTERNAL_ASSERT OR CELL_CORE_USE_PANIC_ASSERT)
        message(STATUS "[Core] Assertions stubbed.")
    endif ()

    target_compile_definitions(CellCore PUBLIC "CELL_CORE_SKIP_ASSERT=1")
elseif (CELL_CORE_USE_EXTERNAL_ASSERT)
    if (CELL_CORE_USE_PANIC_ASSERT)
        message(STATUS "[Core] External assert overrides using Cell::System::Panic.")
    endif ()

    target_compile_definitions(CellCore PUBLIC "CELL_CORE_USE_EXTERNAL_ASSERT=1")
elseif (CELL_CORE_USE_PANIC_ASSERT)
    target_compile_definitions(CellCore PUBLIC "CELL_CORE_USE_PANIC_ASSERT=1")
else ()
    message(STATUS "[Core] Redirecting assertion failures to a basic message box handler.")
endif ()

target_include_directories(CellCore PUBLIC ${CORE_INCLUDES_DIR} ${GENERATE_DIR}/Includes)

if (MSVC) # for the pitiable that do use VS, I hope you know to use Clang at least
    target_compile_options(CellCore PUBLIC
        -W4
        -WX

        -Wno-c99-designator
        -Wno-nullability-completeness
        -Wno-c++2b-extensions

        -Gd
        -Od
        -Zi
    )

    target_compile_definitions(CellCore PUBLIC
        "CELL_NONNULL=_Nonnull"
        "CELL_NULLABLE=_Nullable"
        "CELL_FUNCTION_INTERNAL="
        "CELL_INLINE=inline __attribute__((always_inline))"
    )
else ()
    target_compile_options(CellCore PUBLIC
        -Wall
        -Wextra
        -Werror

        -Wno-c99-designator
        -Wno-c99-extensions
        -Wno-nullability-extension
        -Wno-nullability-completeness
        -Wno-c++2b-extensions

        -pedantic

        -fvisibility=hidden

        -fno-exceptions
        -fno-rtti

        -nostdlib++

        -O0
    )

    target_compile_definitions(CellCore PUBLIC
        "CELL_NONNULL=_Nonnull"
        "CELL_NULLABLE=_Nullable"
        "CELL_FUNCTION_INTERNAL=__attribute__((visibility(\"hidden\")))"
        "CELL_INLINE=inline __attribute__((always_inline))"
    )

    target_link_options(CellCore PUBLIC -fuse-ld=lld -nostdlib++)
endif ()

if (WIN32)
    set(PLATFORM_DIR ${CORE_DIR}/Platform/Windows)
elseif (UNIX)
    set(PLATFORM_DIR ${CORE_DIR}/Platform/Linux)
else ()
    message(FATAL_ERROR "Platform not supported.")
endif ()

set(PLATFORM_DIR ${PLATFORM_DIR} PARENT_SCOPE)
add_subdirectory(${PLATFORM_DIR})

CellPlatformBootstrapTarget(CellCore)

if (CELL_ENABLE_TESTS)
    function(cell_add_test name)
        add_executable(CellCore${name}Test ${CMAKE_CURRENT_LIST_DIR}/Tests/${name}.cc)
        target_link_libraries(CellCore${name}Test PRIVATE CellCore)
        add_test(NAME CellCore${name}Test COMMAND CellCore${name}Test WORKING_DIRECTORY ${PROJECT_ROOT_DIR})
        CellPlatformBootstrapTarget(CellCore${name}Test TRUE)
    endfunction()

    cell_add_test(Enumerables)
    cell_add_test(String)
    cell_add_test(System)
    cell_add_test(Utilities)
endif ()

add_custom_command(TARGET CellCore POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CORE_DIR}/Content/NOTICE $<TARGET_FILE_DIR:CellCore>)
