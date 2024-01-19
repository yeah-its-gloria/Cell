// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Reference.hh>
#include <Cell/Shell/Shell.hh>
#include <Cell/System/String.hh>

class Editor : Cell::Object {
public:
    CELL_INLINE Editor() { }
    CELL_FUNCTION_INTERNAL ~Editor();
    CELL_FUNCTION_INTERNAL void Launch(const Cell::System::String& parameterString);

private:
    Cell::Shell::IShell* shell = nullptr;
};
