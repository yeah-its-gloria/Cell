# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

find_package(Vulkan REQUIRED FATAL_ERROR)

set(submodule_include_dir ${CMAKE_CURRENT_LIST_DIR}/Includes)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH};${CMAKE_CURRENT_LIST_DIR}/BuildScripts)

include(VulkanShader)

target_sources(CellRenderer PRIVATE
    ${submodule_include_dir}/Cell/Renderer/Vulkan/Buffer.hh
    ${submodule_include_dir}/Cell/Renderer/Vulkan/CommandBuffer.hh
    ${submodule_include_dir}/Cell/Renderer/Vulkan/Device.hh
    ${submodule_include_dir}/Cell/Renderer/Vulkan/Image.hh
    ${submodule_include_dir}/Cell/Renderer/Vulkan/Instance.hh
    ${submodule_include_dir}/Cell/Renderer/Vulkan/Pipeline.hh
    ${submodule_include_dir}/Cell/Renderer/Vulkan/RenderTarget.hh
    ${submodule_include_dir}/Cell/Renderer/Vulkan/WSITarget.hh

    ${submodule_include_dir}/Cell/Renderer/Vulkan/CommandParameters/Binding.hh
    ${submodule_include_dir}/Cell/Renderer/Vulkan/CommandParameters/Copy.hh
    ${submodule_include_dir}/Cell/Renderer/Vulkan/CommandParameters/Draw.hh
    ${submodule_include_dir}/Cell/Renderer/Vulkan/CommandParameters/Rendering.hh

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

target_compile_definitions(CellRenderer PUBLIC "CELL_MODULES_RENDERER_VULKAN_AVAILABLE=1")
target_include_directories(CellRenderer PUBLIC ${submodule_include_dir} ${Vulkan_INCLUDE_DIRS})
target_link_libraries(CellRenderer PRIVATE ${Vulkan_LIBRARIES})

# TODO: fix CMake's weirdness about targets and commands following it

if (WIN32)
    target_sources(CellRenderer PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Sources/WSITarget/Platform/Windows.cc)

    # if (DEFINED Vulkan_BINARIES)
    #     add_custom_command(TARGET CellRenderer POST_BUILD
    #         COMMENT "Copying Vulkan loader..."
    #         COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Vulkan_BINARIES} $<TARGET_FILE_DIR:CellRenderer>
    #     )
    # endif ()
elseif (APPLE)
    target_sources(CellRenderer PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Sources/WSITarget/Platform/macOS.cc)

    # if (DEFINED Vulkan_BINARIES)
    #     add_custom_command(TARGET CellRenderer POST_BUILD
    #         COMMENT "Copying Vulkan loader..."
    #         COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Vulkan_BINARIES} $<TARGET_FILE_DIR:CellRenderer>/libvulkan.1.dylib
    #     )
    # endif ()
elseif (UNIX)
    target_sources(CellRenderer PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Sources/WSITarget/Platform/Linux.cc)
else ()
    message(FATAL_ERROR "Unsupported platform.")
endif ()
