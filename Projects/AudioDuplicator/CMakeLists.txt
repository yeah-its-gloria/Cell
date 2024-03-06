# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(CellAudioDuplicator LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Cell - AudioDuplicator Project")

set(SOURCES_DIR ${CMAKE_CURRENT_LIST_DIR}/Sources)

add_executable(CellAudioDuplicator
    ${SOURCES_DIR}/Main.cc
)

target_link_libraries(CellAudioDuplicator PRIVATE
    CellCore
    CellAudio
    CellShell
)

cell_target_bootstrap(CellAudioDuplicator)
