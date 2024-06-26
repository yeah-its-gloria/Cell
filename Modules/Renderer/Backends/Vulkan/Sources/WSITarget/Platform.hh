// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Renderer/Vulkan/WSITarget.hh>

namespace Cell::Renderer::Vulkan {

CELL_FUNCTION_INTERNAL Result createPlatformSurface(VkSurfaceKHR* surface, VkInstance instance, Shell::IShell* shell);

}
