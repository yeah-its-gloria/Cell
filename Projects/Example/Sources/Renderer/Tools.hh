// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "../Example.hh"

#include <Cell/Collection/List.hh>
#include <Cell/Mathematics/Matrix4x4.hh>
#include <Cell/Vulkan/Buffer.hh>
#include <Cell/Vulkan/CommandBuffer.hh>
#include <Cell/Vulkan/Image.hh>
#include <Cell/Vulkan/Pipeline.hh>
#include <Cell/Vulkan/RenderTarget.hh>

struct CELL_PACKED(16) ExampleUBO {
    Cell::Mathematics::Matrix4x4 model;
    Cell::Mathematics::Matrix4x4 view;
    Cell::Mathematics::Matrix4x4 projection;

    float delta;
};

CELL_FUNCTION_INTERNAL void VulkanToolsLoadShader(Cell::Vulkan::Pipeline* CELL_NONNULL pipeline, const Cell::String& path);
CELL_FUNCTION_INTERNAL Cell::Vulkan::Image* VulkanToolsLoadTexture(Cell::Vulkan::Device* CELL_NONNULL device, const Cell::String& texturePath);

CELL_FUNCTION_INTERNAL void VulkanToolsGenerateRenderCommands(const uint32_t vertexCount,
                                                              const uint32_t drawCount,
                                                              Cell::Vulkan::CommandBuffer* CELL_NONNULL commandBuffer,
                                                              Cell::Vulkan::Pipeline* CELL_NONNULL pipeline,
                                                              Cell::Vulkan::Buffer* CELL_NONNULL buffer,
                                                              Cell::Vulkan::IRenderTarget* CELL_NONNULL target,
                                                              const uint32_t frameId
);

CELL_FUNCTION_INTERNAL void VulkanToolsSetUpResources(Cell::Vulkan::Pipeline* CELL_NONNULL pipeline,
                                                      Cell::Collection::List<Cell::Vulkan::Buffer*>& uniforms,
                                                      Cell::Vulkan::Image* CELL_NONNULL texture1,
                                                      Cell::Vulkan::Image* CELL_NONNULL texture2,
                                                      Cell::Vulkan::IRenderTarget* CELL_NONNULL target
);
