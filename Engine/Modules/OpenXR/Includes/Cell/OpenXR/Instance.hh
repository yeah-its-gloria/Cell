// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Mathematics/Vector3.hh>
#include <Cell/Mathematics/Vector4.hh>
#include <Cell/OpenXR/Result.hh>
#include <Cell/Vulkan/Instance.hh>

#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>

#include <math.h>

namespace Cell::OpenXR {

constexpr uint8_t ViewCount = 2;

enum class FormFactor : uint8_t {
    HeadMountedDisplay
};

enum class SessionState : uint8_t {
    Idle,
    Ready,
    Synchronized,
    Visible,
    Focussed,
    Stopping,
    Ending,
    Exiting
};

class Instance : public Object {
friend class VulkanTarget;

public:
    // Creates a new instance, and initializes a Vulkan instance for it.
    CELL_FUNCTION static Wrapped<Instance*, Result> New();

    // Destructor for instances.
    CELL_FUNCTION ~Instance();

    // Initializes the available system, given it's an HMD.
    CELL_FUNCTION Result InitializeSystem(FormFactor factor = FormFactor::HeadMountedDisplay);

    // Creates a session with the given renderer.
    CELL_FUNCTION Result InitializeSessionVulkan();

    // Fetches the view configuration.
    CELL_FUNCTION Result FetchViewConfiguration();

    // Sets up render targets.
    CELL_FUNCTION Result CreateRenderTargetsVulkan();

    // Creates views.
    CELL_FUNCTION Result CreateViews();

    // Creates a reference stage space.
    CELL_FUNCTION Result CreateSpace();

    // Updates the state of the session.
    CELL_FUNCTION Result UpdateState();

    // Begins rendering for the session.
    CELL_FUNCTION Result BeginSession();

    // Ends rendering for the session.
    CELL_FUNCTION Result EndSession();

    // Begins rendering.
    CELL_FUNCTION Result BeginFrame();

    // Ends rendering.
    CELL_FUNCTION Result EndFrame();

    // Requests session shutdown.
    CELL_FUNCTION Result RequestExit();

    // Returns the state of the session.
    CELL_FUNCTION SessionState GetSessionState();

    // Returns a handle to the Vulkan instance.
    CELL_INLINE Vulkan::Instance* GetVulkan() { return this->vulkan; }

    // Returns a handle to a Vulkan render target.
    CELL_INLINE VulkanTarget* GetVulkanTarget(const uint8_t index) { return this->vulkanTargets[index]; }

    CELL_INLINE Mathematics::Vector3 GetViewPosition(const uint8_t side) {
        return Mathematics::Vector3 { this->views[side].pose.position.x, this->views[side].pose.position.y, this->views[side].pose.position.z };
    }

    CELL_INLINE Mathematics::Vector4 GetViewOrientation(const uint8_t side) {
        return Mathematics::Vector4 { this->views[side].pose.orientation.x, this->views[side].pose.orientation.y, this->views[side].pose.orientation.z, this->views[side].pose.orientation.w };
    }

    CELL_INLINE float GetFoVForEye(const uint8_t side) {
        return tanf(this->views[side].fov.angleRight) - tanf(this->views[side].fov.angleLeft);
    }

private:
    CELL_FUNCTION_INTERNAL Instance(XrInstance CELL_NONNULL instance) : instance(instance) { }

    CELL_FUNCTION_INTERNAL Result GetFunctionPointer(const char* CELL_NONNULL name, PFN_xrVoidFunction* function);

    XrInstance instance;

    XrSystemProperties properties = { XR_TYPE_SYSTEM_PROPERTIES, nullptr, 0, 0, { }, { 0, 0, 0 }, { 0, 0 } };
    XrSystemId systemId = XR_NULL_SYSTEM_ID;

    XrViewConfigurationView viewConfigs[ViewCount] =
        {
            { XR_TYPE_VIEW_CONFIGURATION_VIEW, nullptr, 0, 0, 0, 0, 0, 0 },
            { XR_TYPE_VIEW_CONFIGURATION_VIEW, nullptr, 0, 0, 0, 0, 0, 0 }
        };

    XrView views[ViewCount] =
        {
            { XR_TYPE_VIEW, nullptr, { { 0.f, 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f } }, { 0.f, 0.f, 0.f, 0.f } },
            { XR_TYPE_VIEW, nullptr, { { 0.f, 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f } }, { 0.f, 0.f, 0.f, 0.f } }
        };

    XrSpace space = nullptr;

    XrSession session = nullptr;
    XrSessionState state = XR_SESSION_STATE_UNKNOWN;

    Vulkan::Instance* vulkan = nullptr;
    VulkanTarget* vulkanTargets[ViewCount] = { nullptr, nullptr };

    XrFrameState frameState = { XR_TYPE_FRAME_STATE, nullptr, 0, 0, XR_FALSE };
};

}
