# SPDX-FileCopyrightText: Copyright 2023 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(CellModulesAudio LANGUAGES C CXX VERSION 1.0.0 DESCRIPTION "Cell - Modules - Audio")

set(MODULE_INCLUDES_DIR ${CMAKE_CURRENT_LIST_DIR}/Includes)

add_library(CellModulesAudio ${CELL_LIBRARY_TYPE}
            ${MODULE_INCLUDES_DIR}/Cell/Audio/Engine.hh
            ${MODULE_INCLUDES_DIR}/Cell/Audio/PulseAudio.hh
            ${MODULE_INCLUDES_DIR}/Cell/Audio/Result.hh
            ${MODULE_INCLUDES_DIR}/Cell/Audio/WASAPI.hh

            ${CMAKE_CURRENT_LIST_DIR}/Sources/CreateEngine.cc
)

target_compile_definitions(CellModulesAudio PUBLIC "CELL_MODULES_AUDIO_AVAILABLE=1")
target_include_directories(CellModulesAudio PUBLIC ${MODULE_INCLUDES_DIR})
target_link_libraries(CellModulesAudio PUBLIC CellCore)

if (WIN32)
    target_sources(CellModulesAudio PRIVATE
                   ${CMAKE_CURRENT_LIST_DIR}/Sources/WASAPI/BufferInfo.cc
                   ${CMAKE_CURRENT_LIST_DIR}/Sources/WASAPI/CreateRenderClient.cc
                   ${CMAKE_CURRENT_LIST_DIR}/Sources/WASAPI/FindRenderDevice.cc
                   ${CMAKE_CURRENT_LIST_DIR}/Sources/WASAPI/New.cc
                   ${CMAKE_CURRENT_LIST_DIR}/Sources/WASAPI/PlaybackControl.cc
                   ${CMAKE_CURRENT_LIST_DIR}/Sources/WASAPI/SetUpRendering.cc
                   ${CMAKE_CURRENT_LIST_DIR}/Sources/WASAPI/WASAPI.cc
                   ${CMAKE_CURRENT_LIST_DIR}/Sources/WASAPI/WriteSamples.cc
    )

    target_compile_options(CellModulesAudio PUBLIC -Wno-language-extension-token)
    target_link_libraries(CellModulesAudio PRIVATE mmdevapi)
elseif (UNIX)
    find_package(PulseAudio REQUIRED FATAL_ERROR)

    target_sources(CellModulesAudio PRIVATE
                   ${CMAKE_CURRENT_LIST_DIR}/Sources/PulseAudio/New.cc
                   ${CMAKE_CURRENT_LIST_DIR}/Sources/PulseAudio/PulseAudio.cc
                   ${CMAKE_CURRENT_LIST_DIR}/Sources/PulseAudio/SetUpRendering.cc
    )

    target_compile_definitions(CellModulesAudio PRIVATE _REENTRANT=1)
    target_compile_options(CellModulesAudio PRIVATE -pthread)
    target_include_directories(CellModulesAudio PUBLIC ${PulseAudio_INCLUDE_DIRS})
    target_link_libraries(CellModulesAudio PRIVATE ${PulseAudio_LIBRARIES})
else ()
    message(FATAL_ERROR "Unsupported platform")
endif ()

Cell_Platform_Bootstrap_Target(CellModulesAudio)
