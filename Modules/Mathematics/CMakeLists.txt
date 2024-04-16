# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(CellMathematics LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Cell - Modules - Mathematics")

set(MODULE_INCLUDES_DIR ${CMAKE_CURRENT_LIST_DIR}/Includes)

add_library(CellMathematics ${CELL_LIBRARY_TYPE}
    ${MODULE_INCLUDES_DIR}/Cell/Mathematics/ExtendedIntegers.hh
    ${MODULE_INCLUDES_DIR}/Cell/Mathematics/Matrix4x4.hh
    ${MODULE_INCLUDES_DIR}/Cell/Mathematics/Quaternion.hh
    ${MODULE_INCLUDES_DIR}/Cell/Mathematics/Utilities.hh
    ${MODULE_INCLUDES_DIR}/Cell/Mathematics/Vector2.hh
    ${MODULE_INCLUDES_DIR}/Cell/Mathematics/Vector3.hh

# ------------------------------------------------------------------------------------------------

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Checks.cc

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Matrix4x4/Shared.hh

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Matrix4x4/AffineTransforms.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Matrix4x4/Constructors.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Matrix4x4/Functions.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Matrix4x4/Operations.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Matrix4x4/Utilities.cc

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Quaternion/Constructors.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Quaternion/Functions.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Quaternion/Operations.cc

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Vector2/Constructors.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Vector2/Functions.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Vector2/Operations.cc

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Vector3/Constructors.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Vector3/Functions.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Vector3/Operations.cc
)

target_compile_definitions(CellMathematics PUBLIC "CELL_MODULES_MATHEMATICS_AVAILABLE=1")
target_include_directories(CellMathematics PUBLIC ${MODULE_INCLUDES_DIR})
target_link_libraries(CellMathematics PUBLIC CellCore)

if ((WIN32 OR UNIX) AND (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "AMD64" OR ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64"))
    target_compile_definitions(CellMathematics PUBLIC "CELL_MODULES_MATHEMATICS_HARDWARE_ACCELERATE_X86=1")
    target_compile_options(CellMathematics PRIVATE -mfma)
elseif (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "arm64")
    # ...
else ()
    message(FATAL_ERROR "Unsupported platform.")
endif ()

if (CELL_ENABLE_TESTS)
    add_executable(CellMathematicsVectorTest ${CMAKE_CURRENT_LIST_DIR}/Tests/Vector.cc)
    target_link_libraries(CellMathematicsVectorTest PRIVATE CellMathematics)
    add_test(NAME CellMathematicsVectorTest COMMAND CellMathematicsVectorTest WORKING_DIRECTORY ${PROJECT_ROOT_DIR})
    cell_target_bootstrap(CellMathematicsVectorTest TRUE)
endif ()

cell_target_bootstrap(CellMathematics)
