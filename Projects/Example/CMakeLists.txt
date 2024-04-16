# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(CellExample LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Cell - Example Project")

set(BUILDCONTENT_DIR ${CMAKE_CURRENT_LIST_DIR}/BuildContent)
set(CONTENT_DIR ${CMAKE_CURRENT_LIST_DIR}/Content)
set(SOURCES_DIR ${CMAKE_CURRENT_LIST_DIR}/Sources)

add_executable(CellExample
    ${SOURCES_DIR}/Example.hh
    ${SOURCES_DIR}/Example.cc

    ${SOURCES_DIR}/InputController.hh
    ${SOURCES_DIR}/InputController.cc

    #${SOURCES_DIR}/AudioThread.cc

    ${SOURCES_DIR}/Renderer/Thread.cc
    ${SOURCES_DIR}/Renderer/Tools.hh
    ${SOURCES_DIR}/Renderer/Tools/GenerateRenderCommands.cc
    ${SOURCES_DIR}/Renderer/Tools/LoadShader.cc
    ${SOURCES_DIR}/Renderer/Tools/LoadTexture.cc
    ${SOURCES_DIR}/Renderer/Tools/SetUpResources.cc
)

target_link_libraries(CellExample PRIVATE
    CellCore
    #CellAudio
    CellDataManagement
    CellMathematics
    CellShell
    CellVulkan
)

if (TARGET CellOpenXR AND TARGET CellD3D12)
    target_sources(CellExample PRIVATE
        ${SOURCES_DIR}/XR/Thread.cc
    )

    target_link_libraries(CellExample PRIVATE CellOpenXR CellD3D12)
endif ()

cell_target_bootstrap(CellExample)

#CellVulkanAddShader(CellExample DefaultVertex ${BUILDCONTENT_DIR}/Shaders/Default/Vertex.hlsl ${GENERATE_DIR}/Shaders VERTEX)
#CellVulkanAddShader(CellExample DefaultFragment ${BUILDCONTENT_DIR}/Shaders/Default/Fragment.hlsl ${GENERATE_DIR}/Shaders FRAGMENT)
#CellVulkanLinkShaders(CellExample Default ${GENERATE_DIR}/Shaders/DefaultVertex.spv ${GENERATE_DIR}/Shaders/DefaultFragment.spv ${CONTENT_DIR}/Shaders)

CellVulkanAddShader(CellExample DefaultVertex ${BUILDCONTENT_DIR}/Shaders/Default/Vertex.hlsl ${CONTENT_DIR}/Shaders VERTEX)
CellVulkanAddShader(CellExample DefaultFragment ${BUILDCONTENT_DIR}/Shaders/Default/Fragment.hlsl ${CONTENT_DIR}/Shaders FRAGMENT)

CellVulkanAddShader(CellExample Default2Vertex ${BUILDCONTENT_DIR}/Shaders/Default2/Vertex.glsl ${CONTENT_DIR}/Shaders VERTEX TRUE)
CellVulkanAddShader(CellExample Default2Fragment ${BUILDCONTENT_DIR}/Shaders/Default2/Fragment.glsl ${CONTENT_DIR}/Shaders FRAGMENT TRUE)

CellAddContentToPack(CellExample Content ${CONTENT_DIR})
