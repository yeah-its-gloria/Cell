# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(CellVulkan LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Cell - Modules - Vulkan")

find_package(Vulkan REQUIRED FATAL_ERROR)

set(MODULE_INCLUDES_DIR ${CMAKE_CURRENT_LIST_DIR}/Includes)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH};${CMAKE_CURRENT_LIST_DIR}/BuildScripts)

include(VulkanShader)

add_library(CellVulkan ${CELL_LIBRARY_TYPE}
    ${MODULE_INCLUDES_DIR}/Cell/Vulkan/Buffer.hh
    ${MODULE_INCLUDES_DIR}/Cell/Vulkan/CommandBuffer.hh
    ${MODULE_INCLUDES_DIR}/Cell/Vulkan/Device.hh
    ${MODULE_INCLUDES_DIR}/Cell/Vulkan/Image.hh
    ${MODULE_INCLUDES_DIR}/Cell/Vulkan/Instance.hh
    ${MODULE_INCLUDES_DIR}/Cell/Vulkan/Pipeline.hh
    ${MODULE_INCLUDES_DIR}/Cell/Vulkan/RenderTarget.hh
    ${MODULE_INCLUDES_DIR}/Cell/Vulkan/Result.hh
    ${MODULE_INCLUDES_DIR}/Cell/Vulkan/WSITarget.hh

    ${MODULE_INCLUDES_DIR}/Cell/Vulkan/CommandParameters/Binding.hh
    ${MODULE_INCLUDES_DIR}/Cell/Vulkan/CommandParameters/Copy.hh
    ${MODULE_INCLUDES_DIR}/Cell/Vulkan/CommandParameters/Draw.hh
    ${MODULE_INCLUDES_DIR}/Cell/Vulkan/CommandParameters/Rendering.hh

# ------------------------------------------------------------------------------------------------

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Buffer/Buffer.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Buffer/Operations.cc

# ------------------------------------------------------------------------------------------------

    ${CMAKE_CURRENT_LIST_DIR}/Sources/CommandBuffer/CommandBuffer.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/CommandBuffer/Create.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/CommandBuffer/Submit.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/CommandBuffer/SubmitRender.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/CommandBuffer/Write.cc

# ------------------------------------------------------------------------------------------------

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Device/CreateImageView.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Device/Device.cc

# ------------------------------------------------------------------------------------------------

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Image/Image.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Image/CopyDataFromBuffer.cc

# ------------------------------------------------------------------------------------------------

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Instance/Instance.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Instance/QueryPhysicalDevice.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Instance/ScorePhysicalDevice.cc

# ------------------------------------------------------------------------------------------------

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Pipeline/AddMultiShader.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Pipeline/AddResources.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Pipeline/AddShader.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Pipeline/Finalize.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Pipeline/Pipeline.cc

# ------------------------------------------------------------------------------------------------

    ${CMAKE_CURRENT_LIST_DIR}/Sources/WSITarget/Platform.hh

    ${CMAKE_CURRENT_LIST_DIR}/Sources/WSITarget/AcquireNext.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/WSITarget/Create.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/WSITarget/Present.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/WSITarget/Recreate.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/WSITarget/WSITarget.cc

    ${CMAKE_CURRENT_LIST_DIR}/Sources/WSITarget/DeviceUtilities/CreateDepthStencilImage.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/WSITarget/DeviceUtilities/CreateSwapchainForSurface.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/WSITarget/DeviceUtilities/GetSurfaceData.cc
)

target_compile_definitions(CellVulkan PUBLIC "CELL_MODULES_VULKAN_AVAILABLE=1")
target_include_directories(CellVulkan PUBLIC ${MODULE_INCLUDES_DIR} ${Vulkan_INCLUDE_DIRS})
target_link_libraries(CellVulkan PUBLIC CellCore CellMathematics CellShell PRIVATE ${Vulkan_LIBRARIES})

if (WIN32)
    target_sources(CellVulkan PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Sources/WSITarget/Platform/Windows.cc)

    if (DEFINED Vulkan_BINARIES)
        add_custom_command(TARGET CellVulkan POST_BUILD
                           COMMENT "Copying Vulkan loader..."
                           COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Vulkan_BINARIES} $<TARGET_FILE_DIR:CellVulkan>
        )
    endif ()
elseif (APPLE)
    target_sources(CellVulkan PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Sources/WSITarget/Platform/macOS.cc)
elseif (UNIX)
    target_sources(CellVulkan PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Sources/WSITarget/Platform/Linux.cc)
else ()
    message(FATAL_ERROR "Unsupported platform.")
endif ()

cell_target_bootstrap(CellVulkan)
