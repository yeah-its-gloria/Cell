// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/OpenXR/Result.hh>

#ifdef CELL_MODULES_RENDERER_D3D12_AVAILABLE
#include <Cell/Renderer/D3D12/Device.hh>
#endif

#ifdef CELL_MODULES_RENDERER_VULKAN_AVAILABLE
#include <Cell/Renderer/Vulkan/Device.hh>
#endif

#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>

namespace Cell::OpenXR {

enum class FormFactor : uint8_t {
    HeadMountedDisplay
};

// Represents an OpenXR main instance.
class Instance : public NoCopyObject {
friend class Session;

public:
    // Connects to an available runtime and initializes OpenXR.
    CELL_FUNCTION static Wrapped<Instance*, Result> New();

    // Destroys this instance and disconnects from the runtime.
    CELL_FUNCTION ~Instance();

#ifdef CELL_MODULES_RENDERER_D3D12_AVAILABLE
    // Initializes a session with D3D12.
    CELL_FUNCTION Wrapped<Session*, Result> CreateSessionD3D12(const FormFactor formFactor, Renderer::D3D12::Device* device);
#endif

#ifdef CELL_MODULES_RENDERER_VULKAN_AVAILABLE
    // Initializes a session with Vulkan.
    CELL_FUNCTION Wrapped<Session*, Result> CreateSessionVulkan(const FormFactor formFactor, Renderer::Vulkan::Device* device);
#endif

private:
    struct SessionPrepData {
        XrSystemId id;
        XrSystemProperties properties;
        XrViewConfigurationView viewConfigurations[2];
    };

    CELL_FUNCTION_INTERNAL Instance(XrInstance CELL_NONNULL instance) : instance(instance) { }

    CELL_FUNCTION_INTERNAL Wrapped<SessionPrepData, Result> PrepSession(const FormFactor formFactor);
    CELL_FUNCTION_INTERNAL Wrapped<XrSpace, Result> PrepSessionSpace(XrSession session);

    XrInstance instance;
};

}
