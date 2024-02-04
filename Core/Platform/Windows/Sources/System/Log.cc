// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Platform/Windows/Includes.h>

namespace Cell::System {

void Log(const String& message) {
    String logMessage = "LogMessage: ";
    logMessage += message;
    logMessage += "\n";

#ifdef CELL_PLATFORM_WINDOWS_LOG_TO_CONSOLE
    ScopedBlock logMessageStr = logMessage.ToCharPointer();
    printf("%s", logMessageStr);
#else
    ScopedBlock logMessageStr = logMessage.ToPlatformWideString();
    OutputDebugStringW(logMessageStr);
#endif
}

}
