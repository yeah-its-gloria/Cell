# SPDX-FileCopyrightText: Copyright 2023 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(CellVulkan LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Cell - Modules - Vulkan")

find_package(Vulkan REQUIRED FATAL_ERROR)

set(MODULE_INCLUDES_DIR ${CMAKE_CURRENT_LIST_DIR}/Includes)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH};${CMAKE_CURRENT_LIST_DIR}/BuildScripts)

include(VulkanShader)

add_library(CellVulkan ${CELL_LIBRARY_TYPE}
    ${MODULE_INCLUDES_DIR}/Cell/Vulkan/Buffer.hh
    ${MODULE_INCLUDES_DIR}/Cell/Vulkan/CommandBufferManager.hh
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

# ------------------------------------------------------------------------------------------------

    ${CMAKE_CURRENT_LIST_DIR}/Sources/CommandBufferManager/CommandBufferManager.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/CommandBufferManager/CommandOperations.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/CommandBufferManager/CreateBuffers.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/CommandBufferManager/Reset.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/CommandBufferManager/Submit.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/CommandBufferManager/WriteCommands.cc

# ------------------------------------------------------------------------------------------------

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Image/Image.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Image/CopyDataFromBuffer.cc

# ------------------------------------------------------------------------------------------------

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Instance/Internal.hh

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Instance/CreateBuffer.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Instance/CreateCommandBuffer.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Instance/CreateDevice.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Instance/CreateImage.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Instance/CreateImageView.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Instance/CreatePipeline.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Instance/CreateWSITarget.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Instance/GetMemoryTypeIndex.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Instance/InitializeDevice.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Instance/Instance.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Instance/New.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Instance/QueryPhysicalDevice.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Instance/QueryPhysicalDeviceQueues.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Instance/RenderImage.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Instance/ScorePhysicalDevice.cc

# ------------------------------------------------------------------------------------------------

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Pipeline/AddResources.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Pipeline/AddShader.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Pipeline/Finalize.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Pipeline/Pipeline.cc

# ------------------------------------------------------------------------------------------------

    ${CMAKE_CURRENT_LIST_DIR}/Sources/WSITarget/AcquireNext.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/WSITarget/CreateSwapchain.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/WSITarget/PrepareSwapchain.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/WSITarget/Present.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/WSITarget/Recreate.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/WSITarget/RetrieveProperties.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/WSITarget/SetUpSynchronization.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/WSITarget/SetUpDepthBuffer.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/WSITarget/SetUpRendering.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/WSITarget/WSITarget.cc
)

target_compile_definitions(CellVulkan PUBLIC "CELL_MODULES_VULKAN_AVAILABLE=1")
target_include_directories(CellVulkan PUBLIC ${MODULE_INCLUDES_DIR} ${Vulkan_INCLUDE_DIRS})
target_link_libraries(CellVulkan PUBLIC CellCore CellMathematics CellShell PRIVATE ${Vulkan_LIBRARIES})

if (WIN32)
    target_sources(CellVulkan PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Sources/Instance/CreatePlatformSurfaceWindows.cc)

    if (DEFINED Vulkan_BINARIES)
        add_custom_command(TARGET CellVulkan POST_BUILD
                           COMMENT "Copying Vulkan loader..."
                           COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Vulkan_BINARIES} $<TARGET_FILE_DIR:CellVulkan>
        )
    endif ()
elseif (UNIX)
    target_sources(CellVulkan PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Sources/Instance/CreatePlatformSurfaceLinux.cc)
else ()
    message(FATAL_ERROR "Unsupported platform")
endif ()

CellPlatformBootstrapTarget(CellVulkan)
