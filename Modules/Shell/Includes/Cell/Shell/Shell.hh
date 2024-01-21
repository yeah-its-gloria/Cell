// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Collection/List.hh>
#include <Cell/Wrapped.hh>
#include <Cell/Shell/Controller.hh>
#include <Cell/Shell/Result.hh>
#include <Cell/System/String.hh>

namespace Cell::Shell {

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
    virtual Result SetNewTitle(const System::String& title) = 0;

    // Dispatches all updates and fetches new data.
    CELL_FUNCTION Result RunDispatch();

    // Checks whether the title should still be active.
    CELL_NODISCARD CELL_INLINE bool IsStillActive() const {
        return !this->isDone;
    }

    // Returns whether the application is still in the foreground.
    CELL_NODISCARD CELL_INLINE bool IsInForeground() const {
        return this->isActivated;
    }

    // Registers an action for keyboard button interactions.
    CELL_FUNCTION Result RegisterAction(const KeyboardButton button, const ButtonFunction function, void* userData);

    // Registers an action for mouse button interactions.
    CELL_FUNCTION Result RegisterAction(const MouseButton button, const ButtonFunction function, void* userData);

    // Registers an action for controller button interactions.
    CELL_FUNCTION Result RegisterAction(const ControllerButton button, const ButtonFunction function, void* userData);

    // Registers an action for mouse axis (e.g movement, wheel turns) interactions.
    CELL_FUNCTION Result RegisterAction(const MouseAxis axis, const AxisFunction function, void* userData);

    // Registers an action for controller axis (e.g sticks) interactions.
    CELL_FUNCTION Result RegisterAction(const ControllerAxis axis, const AxisFunction function, void* userData);

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
CELL_FUNCTION Wrapped<IShell*, Result> CreateShell(const System::String& title = "");

}
