// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Log.hh>
#include <Cell/System/Memory.hh>
#include <Cell/System/Panic.hh>
#include <Cell/System/Platform/Windows/Includes.h>

#include <stdarg.h>
#include <stdio.h>

namespace Cell::System {

void Log(const String& message, ...) {
    CELL_ASSERT(message.GetSize() < 32768);

    char* messageStr = message.ToCharPointer();

    va_list parameters;
    va_start(parameters, messageStr);

    char buffer[32768] = { 0 };
    vsprintf_s(buffer, 32767, messageStr, parameters);

    va_end(parameters);

    FreeMemory(messageStr);

    String logMessage = "LogMessage: ";
    logMessage += buffer;
    logMessage += "\n";

#ifdef CELL_PLATFORM_WINDOWS_LOG_TO_CONSOLE
    const char* logMessageStr = logMessage.ToCharPointer();
    printf("%s", logMessageStr);
#else
    const wchar_t* logMessageStr = logMessage.ToPlatformWideString();
    OutputDebugStringW(logMessageStr);
#endif

    FreeMemory(logMessageStr);
}

}
