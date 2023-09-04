# SPDX-FileCopyrightText: Copyright 2023 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(OpenInput LANGUAGES C CXX)

set(OPENINPUT_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/Includes)
set(OPENINPUT_PRIVATE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/PrivateIncludes)

add_library(OpenInput SHARED
    ${OPENINPUT_INCLUDE_DIR}/openinput/openinput.h

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Instance.cc
)

target_link_libraries(OpenInput PRIVATE CellCore)

target_include_directories(OpenInput PUBLIC ${OPENINPUT_INCLUDE_DIR} PRIVATE ${OPENINPUT_PRIVATE_INCLUDE_DIR})

if(WIN32)
    set_target_properties(OpenInput PROPERTIES OUTPUT_NAME "openinput-1")
endif()

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Example)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Runtime)
