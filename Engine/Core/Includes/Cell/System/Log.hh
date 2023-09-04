// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/System/String.hh>

namespace Cell::System {

// Logs a message.
//
// The target is determined in engine; formatting should be kept minimal.
// Every message is separated as appropriate for the given logging implementation (e.g newlines are appended automatically for a plain text log file)
CELL_FUNCTION void Log(const System::String& message, ...);

}
