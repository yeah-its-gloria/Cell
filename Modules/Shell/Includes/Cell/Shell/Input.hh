// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Collection/List.hh>
#include <Cell/Shell/Controller.hh>
#include <Cell/Shell/Shell.hh>

namespace Cell::Shell {

typedef void (* ButtonFunction)(InputType, void*);
typedef void (* AxisFunction)(double, void*);

// Input as provided by the shell, generally via a keyboard and mouse.
class Input : public Object {
friend IShell;

public:
    CELL_FUNCTION Result RegisterAction(const KeyboardButton button, const ButtonFunction function, void* userData);
    CELL_FUNCTION Result RegisterAction(const MouseButton button, const ButtonFunction function, void* userData);
    CELL_FUNCTION Result RegisterAction(const ControllerButton button, const ButtonFunction function, void* userData);

    CELL_FUNCTION Result RegisterAction(const MouseAxis axis, const AxisFunction function, void* userData);
    CELL_FUNCTION Result RegisterAction(const ControllerAxis axis, const AxisFunction function, void* userData);

    // Checks for all controllers and initializes them.
    CELL_FUNCTION Result DiscoverControllers();

    // Polls for updates.
    CELL_FUNCTION Result Poll();

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

    CELL_INLINE Input(IShell& shell) : shell(shell) { }

    IShell& shell;
    Collection::List<RegisterInfo> registeredFunctions;
    Collection::List<IController*> controllers;
};

}
