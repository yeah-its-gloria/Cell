# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(CellOpenXR LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Cell - Modules - OpenXR")

find_package(OpenXR QUIET)

if (NOT OpenXR_FOUND)
    message(STATUS "[Modules/OpenXR] OpenXR not found, module disabled.")
    return()
endif ()

if (NOT TARGET CellVulkan AND NOT TARGET CellD3D12)
    message(FATAL_ERROR "Neither Vulkan nor D3D12 were found, no backing renderer available")
endif ()

set(MODULE_INCLUDES_DIR ${CMAKE_CURRENT_LIST_DIR}/Includes)

add_library(CellOpenXR ${CELL_LIBRARY_TYPE}
    ${MODULE_INCLUDES_DIR}/Cell/OpenXR/Instance.hh
    ${MODULE_INCLUDES_DIR}/Cell/OpenXR/Result.hh
    ${MODULE_INCLUDES_DIR}/Cell/OpenXR/Session.hh

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Instance.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/PrepSession.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Session.cc
)

target_compile_definitions(CellOpenXR PUBLIC "CELL_MODULES_OPENXR_AVAILABLE=1")
target_include_directories(CellOpenXR PUBLIC ${MODULE_INCLUDES_DIR} ${OpenXR_INCLUDE_DIRS})
target_link_libraries(CellOpenXR PUBLIC CellCore CellMathematics PRIVATE ${OpenXR_LIBRARIES})

if (TARGET CellVulkan)
    target_sources(CellOpenXR PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Sources/CreateSessionVulkan.cc)

    target_compile_definitions(CellOpenXR PUBLIC "XR_USE_GRAPHICS_API_VULKAN=1")
    target_link_libraries(CellOpenXR PUBLIC CellVulkan PRIVATE ${Vulkan_LIBRARIES})
endif ()

if (TARGET CellD3D12)
    target_sources(CellOpenXR PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Sources/CreateSessionD3D12.cc)

    target_compile_definitions(CellOpenXR PUBLIC "XR_USE_GRAPHICS_API_D3D12=1")
    target_link_libraries(CellOpenXR PUBLIC CellD3D12 PRIVATE dxgi PRIVATE d3d12)
endif ()

if (WIN32)
    if (DEFINED OpenXR_BINARIES)
        add_custom_command(TARGET CellOpenXR POST_BUILD
                           COMMENT "Copying OpenXR loader..."
                           COMMAND ${CMAKE_COMMAND} -E copy_if_different ${OpenXR_BINARIES} $<TARGET_FILE_DIR:CellOpenXR>
        )
    endif ()
elseif (NOT UNIX)
    message(FATAL_ERROR "Unsupported platform.")
endif ()

cell_target_bootstrap(CellOpenXR)
