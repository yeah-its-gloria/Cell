# SPDX-FileCopyrightText: Copyright 2023 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH};${ENGINE_DIR}/BuildScripts;${ENGINE_DIR}/BuildScripts/Dependencies)

include(Modules)
include(Shaders)

add_subdirectory(${CORE_DIR})

set(PLATFORM_DIR ${PLATFORM_DIR} PARENT_SCOPE)

CellAddModule(CELL_ENABLE_MODULE_AUDIO          ${MODULES_DIR}/Audio          "Whether the Audio module should be built. Defaults to ON.")
CellAddModule(CELL_ENABLE_MODULE_DATAMANAGEMENT ${MODULES_DIR}/DataManagement "Whether the Data Management module should be built. Defaults to ON.")
CellAddModule(CELL_ENABLE_MODULE_MATHEMATICS    ${MODULES_DIR}/Mathematics    "Whether the Mathematics module should be built. Defaults to ON.")
CellAddModule(CELL_ENABLE_MODULE_OPENXR         ${MODULES_DIR}/OpenXR         "Whether the OpenXR module should be built. Defaults to ON.")
CellAddModule(CELL_ENABLE_MODULE_SHELL          ${MODULES_DIR}/Shell          "Whether the Shell module should be built. Defaults to ON.")
CellAddModule(CELL_ENABLE_MODULE_VULKAN         ${MODULES_DIR}/Vulkan         "Whether the Vulkan module should be built. Defaults to ON.")

if (WIN32)
    CellAddModule(CELL_ENABLE_MODULE_D3D12 ${MODULES_DIR}/D3D12 "Whether the D3D12 module should be built. Defaults to ON. Only available on Windows.")
endif()
