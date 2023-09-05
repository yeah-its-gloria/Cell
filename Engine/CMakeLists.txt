# SPDX-FileCopyrightText: Copyright 2023 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(CellCore LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Cell - Engine")

include(Shared)

include(ContentPacking)
include(Shaders)

add_subdirectory(${CORE_DIR})

set(PLATFORM_DIR ${PLATFORM_DIR} PARENT_SCOPE)

Cell_Add_Module(CELL_ENABLE_MODULE_AUDIO          ${MODULES_DIR}/Audio          "Whether the Audio module should be built. Defaults to ON.")
Cell_Add_Module(CELL_ENABLE_MODULE_DATAMANAGEMENT ${MODULES_DIR}/DataManagement "Whether the Data Management module should be built. Defaults to ON.")
Cell_Add_Module(CELL_ENABLE_MODULE_MATHEMATICS    ${MODULES_DIR}/Mathematics    "Whether the Mathematics module should be built. Defaults to ON.")
Cell_Add_Module(CELL_ENABLE_MODULE_OPENXR         ${MODULES_DIR}/OpenXR         "Whether the OpenXR module should be built. Defaults to ON.")
Cell_Add_Module(CELL_ENABLE_MODULE_SHELL          ${MODULES_DIR}/Shell          "Whether the Shell module should be built. Defaults to ON.")
Cell_Add_Module(CELL_ENABLE_MODULE_VULKAN         ${MODULES_DIR}/Vulkan         "Whether the Vulkan module should be built. Defaults to ON.")

if (CELL_ENABLE_UTILITIES)
    add_subdirectory(${UTILITIES_DIR}/ContentPacker)
endif ()
