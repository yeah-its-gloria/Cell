# SPDX-FileCopyrightText: Copyright 2023 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

cmake_minimum_required(VERSION 3.20.0 FATAL_ERROR)

project(Cell LANGUAGES C CXX VERSION 1.0.0 DESCRIPTION "Cell")

cmake_policy(SET CMP0054 NEW)
cmake_policy(SET CMP0079 NEW)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (NOT ${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
    message(WARNING "The C compiler \"${CMAKE_C_COMPILER_ID}\" is not supported. Using LLVM/clang is strongly recommended instead.")
endif ()

if (NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    message(WARNING "The C++ compiler \"${CMAKE_CXX_COMPILER_ID}\" is not supported. Using LLVM/clang is strongly recommended instead.")
endif ()

if (NOT WIN32 AND NOT UNIX)
    message(WARNING "Support for platforms other than Windows or Linux is considered experimental at the moment.")
endif ()

if (NOT ${CMAKE_GENERATOR} STREQUAL "Ninja Multi-Config" AND NOT ${CMAKE_GENERATOR} STREQUAL "Ninja" AND (NOT MSVC AND NOT ${CMAKE_GENERATOR} STREQUAL "Visual Studio 17 2022"))
    message(AUTHOR_WARNING "Using Ninja or Ninja Multi-Config as the generator is recommended over \"${CMAKE_GENERATOR}\".")
endif ()

if (MSVC AND NOT ${CMAKE_VS_PLATFORM_TOOLSET} STREQUAL "ClangCL")
    message(WARNING "CL is basically guaranteed to break. Recommended that you use the ClangCL tookit instead of \"${CMAKE_VS_PLATFORM_TOOLSET}\".")
endif ()

option(CELL_ENABLE_TESTS "Whether tests should be built. Defaults to ON." ON)
option(CELL_ENABLE_EDITOR "Whether the editor should be built. Defaults to ON." ON)
option(CELL_ENABLE_PROJECTS "Whether projects should be built. Defaults to ON." ON)
option(CELL_ENABLE_UTILITIES "Whether utilities should be built. Defaults to ON." ON)

option(CELL_ENABLE_STATIC_BUILD "Whether the libraries (engine, modules, etc) should be build statically. Defaults to OFF." OFF)

option(CELL_USE_EXTERNAL_ASSERT "Whether to use assert from the C standard library implementation. Defaults to OFF." OFF)
option(CELL_USE_PANIC_ASSERT "Whether to use a built in engine assert implementation. Calls Cell::System::Panic. Defaults to ON." ON)
option(CELL_SKIP_ASSERT "Whether to skip assert checks and simply continue. Defaults to OFF." OFF)

if (WIN32)
    option(CELL_PLATFORM_WINDOWS_LOG_TO_CONSOLE "Whether to log to the console or via OutputDebugStringW" OFF)
endif ()

if (CELL_ENABLE_STATIC_BUILD)
    set(CELL_LIBRARY_TYPE STATIC)
else ()
    set(CELL_LIBRARY_TYPE SHARED)
endif ()

set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

set(ENGINE_DIR ${PROJECT_ROOT_DIR}/Engine)
set(CORE_DIR ${ENGINE_DIR}/Core)

set(CORE_INCLUDES_DIR ${CORE_DIR}/Includes)
set(MODULES_DIR ${ENGINE_DIR}/Modules)
set(UTILITIES_DIR ${ENGINE_DIR}/Utilities)

set(GENERATE_DIR ${CMAKE_CURRENT_BINARY_DIR}/Generated)

set(CMAKE_MODULE_PATH ${ENGINE_DIR}/BuildScripts;${ENGINE_DIR}/BuildScripts/Dependencies)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if (CELL_ENABLE_TESTS)
    enable_testing()
endif ()

file(MAKE_DIRECTORY ${GENERATE_DIR})

add_subdirectory(${ENGINE_DIR})

if (CELL_ENABLE_EDITOR)
    add_subdirectory(${PROJECT_ROOT_DIR}/Editor)
endif ()

if (CELL_ENABLE_PROJECTS)
    add_subdirectory(${PROJECT_ROOT_DIR}/Projects)
endif ()
