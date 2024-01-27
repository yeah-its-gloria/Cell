// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "../Renderer/Tools.hh"

#include <Cell/OpenXR/Instance.hh>
#include <Cell/Vulkan/Buffer.hh>
#include <Cell/Vulkan/CommandBufferManager.hh>
#include <Cell/Vulkan/Image.hh>
#include <Cell/Vulkan/Pipeline.hh>

void XRToolsPrepare(Example* example,
                    Cell::OpenXR::Instance* instance,
                    ExampleUBO& ubo,
                    Cell::Vulkan::Image* texture1,
                    Cell::Vulkan::Image* texture2,
                    Cell::Vulkan::Buffer* buffer,
                    Cell::Collection::List<Cell::Vulkan::Buffer*>& uniforms,
                    Cell::Vulkan::Pipeline* pipeline,
                    Cell::Vulkan::CommandBufferManager* commandBuffer,
                    Cell::OpenXR::VulkanTarget* target);
