// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Collection/List.hh>
#include <Cell/Shell/InputTypes.hh>
#include <Cell/Shell/Result.hh>
#include <Cell/Shell/Shell.hh>

namespace Cell::Shell {

// Represents an input callback.
// Parameters are the matched input value and the user data pointer.
typedef void (*InputFunction)(KeyboardButton, void*);

// Input as provided by the shell, generally via a keyboard and mouse.
class Input : public Object {
friend IShell;

public:
    // Registers an action.
    CELL_FUNCTION Result RegisterAction(const KeyboardButton value, const InputFunction function, void* userData);

    // Polls for updates.
    CELL_FUNCTION Result Poll();

private:
    struct RegisterInfo {
        const KeyboardButton match;
        const InputFunction function;
        void* userData;
    };

    CELL_FUNCTION_INTERNAL Input(IShell& shell) : shell(shell) {}

    IShell& shell;
    Collection::List<RegisterInfo> registeredFunctions;
};

}
