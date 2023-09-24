// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "../Vulkan/Tools.hh"
#include <Cell/OpenXR/Instance.hh>
#include <Cell/Vulkan/Buffer.hh>
#include <Cell/Vulkan/CommandBufferManager.hh>
#include <Cell/Vulkan/Image.hh>
#include <Cell/Vulkan/Pipeline.hh>

void XRToolsPrepare(Example* example,
                    Cell::OpenXR::Instance* instance,
                    ExampleUBO& ubo,
                    Cell::Vulkan::Image* texture,
                    Cell::Vulkan::Buffer* buffer,
                    Cell::List<Cell::Vulkan::Buffer*>& uniforms,
                    Cell::Vulkan::Pipeline* pipeline,
                    Cell::Vulkan::CommandBufferManager* commandBuffer,
                    Cell::OpenXR::VulkanTarget* target);
