// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "../Example.hh"

#include <Cell/Collection/List.hh>
#include <Cell/Mathematics/Matrix4x4.hh>
#include <Cell/Renderer/Vulkan/Buffer.hh>
#include <Cell/Renderer/Vulkan/CommandBuffer.hh>
#include <Cell/Renderer/Vulkan/Image.hh>
#include <Cell/Renderer/Vulkan/Pipeline.hh>
#include <Cell/Renderer/Vulkan/RenderTarget.hh>

struct ExampleUBO {
    Cell::Mathematics::Matrix4x4 model;
    Cell::Mathematics::Matrix4x4 view;
    Cell::Mathematics::Matrix4x4 projection;

    float delta;
};

CELL_FUNCTION_INTERNAL void VulkanToolsLoadShader(Cell::Renderer::Vulkan::Pipeline* CELL_NONNULL pipeline, const Cell::String& path);

CELL_FUNCTION_INTERNAL void VulkanToolsLoadShader(Cell::Renderer::Vulkan::Pipeline* CELL_NONNULL pipeline,
                                                  const Cell::String& path,
                                                  const Cell::Renderer::Vulkan::Stage stage);

CELL_FUNCTION_INTERNAL Cell::Renderer::Vulkan::Image* VulkanToolsLoadTexture(Cell::Renderer::Vulkan::Device* CELL_NONNULL device,
                                                                             const Cell::String& texturePath);

CELL_FUNCTION_INTERNAL void VulkanToolsGenerateRenderCommands(const uint32_t vertexCount,
                                                              const uint32_t drawCount,
                                                              Cell::Renderer::Vulkan::CommandBuffer* CELL_NONNULL commandBuffer,
                                                              Cell::Renderer::Vulkan::Pipeline* CELL_NONNULL pipeline,
                                                              Cell::Renderer::Vulkan::Buffer* CELL_NONNULL buffer,
                                                              Cell::Renderer::Vulkan::IRenderTarget* CELL_NONNULL target,
                                                              const uint32_t frameId);

CELL_FUNCTION_INTERNAL void VulkanToolsSetUpResources(Cell::Renderer::Vulkan::Pipeline* CELL_NONNULL pipeline,
                                                      Cell::Collection::List<Cell::Renderer::Vulkan::Buffer*>& uniforms,
                                                      Cell::Renderer::Vulkan::Image* CELL_NONNULL texture1,
                                                      Cell::Renderer::Vulkan::Image* CELL_NONNULL texture2,
                                                      Cell::Renderer::Vulkan::IRenderTarget* CELL_NONNULL target);
