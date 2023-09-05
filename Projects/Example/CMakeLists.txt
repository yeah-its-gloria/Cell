# SPDX-FileCopyrightText: Copyright 2023 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(CellExample LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Cell - Example")

set(BUILDCONTENT_DIR ${CMAKE_CURRENT_LIST_DIR}/BuildContent)
set(CONTENT_DIR ${CMAKE_CURRENT_LIST_DIR}/Content)
set(SOURCES_DIR ${CMAKE_CURRENT_LIST_DIR}/Sources)

add_executable(CellExample
               ${SOURCES_DIR}/Example.hh
               ${SOURCES_DIR}/Example.cc

               ${SOURCES_DIR}/AudioThread.cc
               ${SOURCES_DIR}/NetworkThread.cc

               ${SOURCES_DIR}/Vulkan/Thread.cc
               ${SOURCES_DIR}/Vulkan/Tools.hh
               ${SOURCES_DIR}/Vulkan/Tools/GenerateRenderCommands.cc
               ${SOURCES_DIR}/Vulkan/Tools/LoadShader.cc
               ${SOURCES_DIR}/Vulkan/Tools/LoadTexture.cc
               ${SOURCES_DIR}/Vulkan/Tools/SetUpInput.cc
               ${SOURCES_DIR}/Vulkan/Tools/SetUpResources.cc

               ${SOURCES_DIR}/XR/Thread.cc
               ${SOURCES_DIR}/XR/Tools.hh
               ${SOURCES_DIR}/XR/ToolsPrepare.cc
)

target_link_libraries(CellExample PRIVATE
                      CellCore
                      CellModulesAudio
                      CellModulesDataManagement
                      CellModulesMathematics
                      CellModulesOpenXR
                      CellModulesShell
                      CellModulesVulkan
)

Cell_Platform_Bootstrap_Target(CellExample)

Cell_Add_Shader(CellExample DefaultVertex ${BUILDCONTENT_DIR}/Shaders/Default/Vertex.hlsl ${CONTENT_DIR}/Shaders/DefaultVertex.spv)
Cell_Add_Shader(CellExample DefaultFragment ${BUILDCONTENT_DIR}/Shaders/Default/Fragment.hlsl ${CONTENT_DIR}/Shaders/DefaultFragment.spv)

Cell_Add_Shader(CellExample Default2Vertex ${BUILDCONTENT_DIR}/Shaders/Default2/Vertex.glsl ${CONTENT_DIR}/Shaders/Default2Vertex.spv TRUE)
Cell_Add_Shader(CellExample Default2Fragment ${BUILDCONTENT_DIR}/Shaders/Default2/Fragment.glsl ${CONTENT_DIR}/Shaders/Default2Fragment.spv TRUE)

Cell_Add_Content_To_Pack(CellExample Content ${CONTENT_DIR})
