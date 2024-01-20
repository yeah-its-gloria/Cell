# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(CellOpenXR LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Cell - Modules - OpenXR")

find_package(OpenXR)

if(NOT OpenXR_FOUND)
    message(STATUS "OpenXR not found, disabling CellOpenXR.")
    return()
endif()

find_package(Vulkan REQUIRED FATAL_ERROR)

set(MODULE_INCLUDES_DIR ${CMAKE_CURRENT_LIST_DIR}/Includes)

add_library(CellOpenXR ${CELL_LIBRARY_TYPE}
    ${MODULE_INCLUDES_DIR}/Cell/OpenXR/Instance.hh
    ${MODULE_INCLUDES_DIR}/Cell/OpenXR/Result.hh
    ${MODULE_INCLUDES_DIR}/Cell/OpenXR/VulkanTarget.hh

# ------------------------------------------------------------------------------------------------

    ${CMAKE_CURRENT_LIST_DIR}/Sources/CreateRenderTargetsVulkan.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/CreateSpace.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/FetchViewConfiguration.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/FrameControl.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/GetFunctionPointer.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/InitializeSessionVulkan.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/InitializeSystem.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Instance.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/New.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Session.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/UpdateState.cc

    ${CMAKE_CURRENT_LIST_DIR}/Sources/VulkanTarget/AcquireAndPresent.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/VulkanTarget/CreateSwapchain.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/VulkanTarget/VulkanTarget.cc
)

target_compile_definitions(CellOpenXR PUBLIC
    "CELL_MODULES_OPENXR_AVAILABLE=1"

    "XR_USE_GRAPHICS_API_VULKAN=1"
    # "XR_EXTENSION_PROTOTYPES=1"
)

target_include_directories(CellOpenXR PUBLIC ${MODULE_INCLUDES_DIR} ${OpenXR_INCLUDE_DIRS} ${Vulkan_INCLUDE_DIRS})

target_link_libraries(CellOpenXR PUBLIC
    CellCore
    CellMathematics
    CellVulkan

    PRIVATE ${OpenXR_LIBRARIES}
    PRIVATE ${Vulkan_LIBRARIES}
)

if(WIN32)
    if(DEFINED OpenXR_BINARIES)
        add_custom_command(TARGET CellOpenXR POST_BUILD
                           COMMENT "Copying OpenXR loader..."
                           COMMAND ${CMAKE_COMMAND} -E copy_if_different ${OpenXR_BINARIES} $<TARGET_FILE_DIR:CellOpenXR>
        )
    endif()
elseif(UNIX)
    # ...
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

CellPlatformBootstrapTarget(CellOpenXR)
