// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/String.hh>
#include <Cell/Collection/List.hh>
#include <Cell/Shell/Controller.hh>

namespace Cell::Shell {

// Indicates the state of the application currently running.
// This can have different effects depending on the platform, e.g on desktops, a spinning cursor is shown for working.
enum class ShellStatus : uint8_t {
    // Indicates that the application is acting as normal.
    Default,

    // Indicates that the application is currently very busy.
    Working
};

// Defines a 2 dimensional size.
struct Extent {
    uint32_t width;
    uint32_t height;
};

// Function prototype for button (boolean-like) interactions.
// Records whether the relevant button has been pressed, released or held.
typedef void (* ButtonFunction)(InputType, void*);

// Function prototype for axis (float-like) interactions.
// Records how far the axis has currently been activated, as long as it is not in its dead zone radius.
typedef void (* AxisFunction)(double, void*);

// Base shell interface.
class IShell : public Object {
public:
    // Common destructor.
    virtual ~IShell() = default;

    // Tells the shell that we'd like to stop running.
    virtual Result RequestQuit() = 0;

    // Gets the count of the drawable area of the current window.
    virtual Wrapped<Extent, Result> GetDrawableExtent() = 0;

    // Sets a new drawable area count for the current window.
    virtual Result SetDrawableExtent(const Extent extent) = 0;

    // Sets a new window title.
    virtual Result SetNewTitle(const String& title) = 0;

    // Sets a new status to be indicated to the user.
    virtual Result IndicateStatus(const ShellStatus status) = 0;

    // Sets whether the cursor should be caught by the window (centered + hidden generally).
    virtual Result CaptureState(const bool captured = true) = 0;

    // Logs text to the window, similar to the regular Log function.
    // Some environments might not properly support this, and instead use the regular log function.
    // Do note that any renderer attached to a shell will clear the window and make any text messages on it disappear.
    virtual Result Log(const String& text) = 0;

    // Clears logged messages on the window, if available.
    virtual Result LogClear() = 0;

    // Dispatches all updates and fetches new data.
    CELL_FUNCTION Result RunDispatch();

    // Checks whether the title should still be active.
    CELL_NODISCARD inline bool IsStillActive() const {
        return !this->isDone;
    }

    // Returns whether the application is still in the foreground.
    CELL_NODISCARD inline bool IsInForeground() const {
        return this->isActivated;
    }

    // Registers an action for keyboard button interactions.
    CELL_FUNCTION void RegisterAction(const KeyboardButton button, const ButtonFunction function, void* userData);

    // Registers an action for mouse button interactions.
    CELL_FUNCTION void RegisterAction(const MouseButton button, const ButtonFunction function, void* userData);

    // Registers an action for controller button interactions.
    CELL_FUNCTION void RegisterAction(const ControllerButton button, const ButtonFunction function, void* userData);

    // Registers an action for mouse axis (e.g movement, wheel turns) interactions.
    CELL_FUNCTION void RegisterAction(const MouseAxis axis, const AxisFunction function, void* userData);

    // Registers an action for controller axis (e.g sticks) interactions.
    CELL_FUNCTION void RegisterAction(const ControllerAxis axis, const AxisFunction function, void* userData);

    // Checks for all supported peripherals on the system.
    //
    // Generally useful to initialize controller support.
    // Keyboard and mouse input is usually delivered by the underlying shell.
    CELL_FUNCTION Result DiscoverPeripherals();

protected:
    virtual Result RunDispatchImpl() = 0;

    bool isActivated = true;
    bool isDone = false;

    KeyboardButton keys = KeyboardButton::None;
    KeyboardButton oldKeys = KeyboardButton::None;

    double mouseX = 0.0;
    double mouseY = 0.0;

private:
    struct RegisterInfo {
        uint8_t type;
        void* userData;

        union {
            const KeyboardButton keyboard;
            const MouseButton mouse;
            const ControllerButton controller;
            const MouseAxis mouseAxis;
            const ControllerAxis controllerAxis;
        };

        union {
            const ButtonFunction button;
            const AxisFunction axis;
        };
    };

    Collection::List<RegisterInfo> registeredFunctions;
    Collection::List<IController*> controllers;
};

// Sets up the most suited shell implementation for the platform.
CELL_FUNCTION Wrapped<IShell*, Result> CreateShell(const String& title = "", const Extent extent = { 1280, 720 });

}
