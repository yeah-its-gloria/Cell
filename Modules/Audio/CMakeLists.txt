# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(CellAudio LANGUAGES C CXX VERSION 1.0.0 DESCRIPTION "Cell - Modules - Audio")

set(MODULE_INCLUDES_DIR ${CMAKE_CURRENT_LIST_DIR}/Includes)

add_library(CellAudio ${CELL_LIBRARY_TYPE}
    ${MODULE_INCLUDES_DIR}/Cell/Audio/Renderer.hh
    ${MODULE_INCLUDES_DIR}/Cell/Audio/Result.hh
    ${MODULE_INCLUDES_DIR}/Cell/Audio/Subsystem.hh

    ${MODULE_INCLUDES_DIR}/Cell/Audio/Implementations/PulseAudio.hh
    ${MODULE_INCLUDES_DIR}/Cell/Audio/Implementations/WASAPI.hh

    ${CMAKE_CURRENT_LIST_DIR}/Sources/CreateSubsystem.cc
)

target_compile_definitions(CellAudio PUBLIC "CELL_MODULES_AUDIO_AVAILABLE=1")
target_include_directories(CellAudio PUBLIC ${MODULE_INCLUDES_DIR})
target_link_libraries(CellAudio PUBLIC CellCore)

if (WIN32)
    target_sources(CellAudio PRIVATE
       ${CMAKE_CURRENT_LIST_DIR}/Sources/WASAPI/NotificationImpl.hh

        ${CMAKE_CURRENT_LIST_DIR}/Sources/WASAPI/CreateRenderer.cc
        ${CMAKE_CURRENT_LIST_DIR}/Sources/WASAPI/NotificationImpl.cc
        ${CMAKE_CURRENT_LIST_DIR}/Sources/WASAPI/Renderer.cc
        ${CMAKE_CURRENT_LIST_DIR}/Sources/WASAPI/Subsystem.cc
    )

    target_compile_options(CellAudio PUBLIC -Wno-language-extension-token)
    target_link_libraries(CellAudio PRIVATE mmdevapi)
elseif (UNIX)
    find_package(PulseAudio REQUIRED FATAL_ERROR)

    target_sources(CellAudio PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Sources/PulseAudio/CreateRenderer.cc
        ${CMAKE_CURRENT_LIST_DIR}/Sources/PulseAudio/Renderer.cc
        ${CMAKE_CURRENT_LIST_DIR}/Sources/PulseAudio/Subsystem.cc
    )

    target_include_directories(CellAudio PUBLIC ${PulseAudio_INCLUDE_DIRS})
    target_link_libraries(CellAudio PRIVATE ${PulseAudio_LIBRARIES})
else ()
    message(FATAL_ERROR "Unsupported platform.")
endif ()

cell_target_bootstrap(CellAudio)
