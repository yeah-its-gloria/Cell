# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

cmake_minimum_required(VERSION 3.20.0 FATAL_ERROR)

project(Cell LANGUAGES C CXX VERSION 1.0.0 DESCRIPTION "Cell")

cmake_policy(SET CMP0054 NEW)
cmake_policy(SET CMP0079 NEW)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH};${PROJECT_ROOT_DIR}/BuildScripts)

include(Modules)

set(CORE_DIR ${PROJECT_ROOT_DIR}/Core)
set(CORE_INCLUDES_DIR ${CORE_DIR}/Includes)

set(MODULES_DIR ${PROJECT_ROOT_DIR}/Modules)

set(GENERATE_DIR ${CMAKE_BINARY_DIR}/Generated)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if (WIN32)
    set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif ()

if (NOT ${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
    message(WARNING "The C compiler \"${CMAKE_C_COMPILER_ID}\" is not supported, using LLVM/clang is strongly recommended instead")
endif ()

if (NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    message(WARNING "The C++ compiler \"${CMAKE_CXX_COMPILER_ID}\" is not supported, using LLVM/clang is strongly recommended instead")
endif ()

if (WIN32)
    if (NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        message(WARNING "\"${CMAKE_SYSTEM_NAME}\" is not a known Win32 implementation")
    endif ()
elseif (UNIX)
    if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
        message(NOTICE "Several interfaces on Linux aren't yet implemented fully!")
    else ()
        message(WARNING "Cell mainly targets Linux, other \"Unix-likes\" (${CMAKE_SYSTEM_NAME}) may or may not be compatible")
    endif ()
else()
    message(WARNING "Unknown environment")
endif ()

if (NOT ${CMAKE_GENERATOR} STREQUAL "Ninja Multi-Config"
        AND NOT ${CMAKE_GENERATOR} STREQUAL "Ninja"
        AND (NOT MSVC AND NOT ${CMAKE_GENERATOR} STREQUAL "Visual Studio 17 2022")
    )

    message(AUTHOR_WARNING "Using Ninja or Ninja Multi-Config as the generator is recommended over \"${CMAKE_GENERATOR}\".")
endif ()

if (MSVC AND NOT ${CMAKE_VS_PLATFORM_TOOLSET} STREQUAL "ClangCL")
    message(WARNING "CL is basically guaranteed to break. It's recommended that you use the ClangCL tookit instead of \"${CMAKE_VS_PLATFORM_TOOLSET}\".")
endif ()

option(CELL_ENABLE_EDITOR "Whether the editor should be built. Defaults to ON." ON)
option(CELL_ENABLE_PROJECT "Whether the in tree project should be built. Defaults to ON." ON)
option(CELL_ENABLE_UTILITIES "Whether utilities should be built. Defaults to ON." ON)

option(CELL_ENABLE_STATIC_BUILD "Whether the libraries (core, modules, etc) should be build statically. Defaults to OFF." OFF)

option(CELL_ENABLE_TESTS "Whether tests should be built. Defaults to ON." ON)
option(CELL_ENABLE_TESTS_LIKELY_FAILURE "Whether tests that are likely to fail should be built. Defaults to OFF." OFF)

if (CELL_ENABLE_STATIC_BUILD)
    set(CELL_LIBRARY_TYPE STATIC)
else ()
    set(CELL_LIBRARY_TYPE SHARED)
endif ()

if (CELL_ENABLE_TESTS)
    enable_testing()
endif ()

file(MAKE_DIRECTORY ${GENERATE_DIR})

add_subdirectory(${CORE_DIR})

CellAddModule(Audio)
CellAddModule(DataManagement)
CellAddModule(JavaScript)
CellAddModule(Mathematics)
CellAddModule(OpenXR)
CellAddModule(Shell)
CellAddModule(Vulkan)

if (WIN32)
    CellAddModule(D3D12)
endif ()

if (CELL_ENABLE_EDITOR)
    add_subdirectory(${PROJECT_ROOT_DIR}/Editor)
endif ()

if (CELL_ENABLE_PROJECT)
    add_subdirectory(${PROJECT_ROOT_DIR}/Project)
endif ()
