// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/String.hh>

namespace Cell::System {

// Logs a message.
//
// The target is determined in core; formatting should be kept minimal.
// Every message is separated as appropriate for the given logging implementation,
//  e.g newlines are appended automatically for a plain text log to console or a file)
CELL_FUNCTION void Log(const String& message);

// Stinky utility because C/C++ really don't like us.
CELL_FUNCTION_TEMPLATE void Log(const char* message) {
    Log(String(message));
}

// Utility to auto-format messages.
template <typename... T> CELL_FUNCTION_TEMPLATE void Log(const char* format, T&&... args) {
    Log(String::Format(format, args...));
}

}
