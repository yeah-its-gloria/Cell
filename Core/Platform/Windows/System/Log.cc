// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Platform/Windows/Includes.h>

#ifdef CELL_PLATFORM_WINDOWS_LOG_TO_CONSOLE
#include <stdio.h>
#endif

namespace Cell::System {

void Log(const String& message) {
    String logMessage = "LogMessage: ";
    logMessage += message;
    logMessage += "\n";

#ifdef CELL_PLATFORM_WINDOWS_LOG_TO_CONSOLE
    ScopedBlock<char> logMessageStr = logMessage.ToCharPointer();
    printf("%s", (char*)logMessageStr);
#else
    ScopedBlock<wchar_t> logMessageStr = logMessage.ToPlatformWideString();
    OutputDebugStringW(logMessageStr);
#endif
}

}
