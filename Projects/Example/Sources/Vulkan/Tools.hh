// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "../Example.hh"

#include <Cell/Mathematics/Matrix4x4.hh>
#include <Cell/Vulkan/Buffer.hh>
#include <Cell/Vulkan/CommandBufferManager.hh>
#include <Cell/Vulkan/Image.hh>
#include <Cell/Vulkan/Pipeline.hh>
#include <Cell/Vulkan/RenderTarget.hh>
#include <Cell/Vulkan/Vertex.hh>

struct CELL_PACKED(16) ExampleUBO {
    Cell::Mathematics::Matrix4x4 model;
    Cell::Mathematics::Matrix4x4 view;
    Cell::Mathematics::Matrix4x4 projection;

    float timeMilliseconds = 0;
};

struct InputData {
    Cell::Mathematics::Vector3* position;
    const Example* instance;
};

CELL_FUNCTION_INTERNAL void VulkanToolsLoadShader(Cell::Vulkan::Pipeline* CELL_NONNULL pipeline, const Cell::System::String& path, const Cell::Vulkan::Stage stage);
CELL_FUNCTION_INTERNAL Cell::Vulkan::Image* VulkanToolsLoadTexture(Cell::Vulkan::Instance* CELL_NONNULL instance, const Cell::System::String& texturePath);
CELL_FUNCTION_INTERNAL void VulkanToolsInputSetUp(Cell::Shell::Input* CELL_NONNULL input, InputData* CELL_NONNULL data);

CELL_FUNCTION_INTERNAL void VulkanToolsGenerateRenderCommands(const uint32_t vertexCount, const uint32_t drawCount,
                                                              Cell::Vulkan::CommandBufferManager* CELL_NONNULL commandBuffer,
                                                              Cell::Vulkan::Pipeline* CELL_NONNULL pipeline,
                                                              Cell::Vulkan::Buffer* CELL_NONNULL buffer,
                                                              Cell::Vulkan::IRenderTarget* CELL_NONNULL target,
                                                              const uint32_t frameId);

CELL_FUNCTION_INTERNAL void VulkanToolsSetUpResources(Cell::Vulkan::Pipeline* CELL_NONNULL pipeline,
                                                      Cell::Vulkan::Buffer* CELL_NONNULL* CELL_NONNULL uniforms,
                                                      Cell::Vulkan::Image* CELL_NONNULL texture,
                                                      Cell::Vulkan::IRenderTarget* CELL_NONNULL target);
