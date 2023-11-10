// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Log.hh>
#include <Cell/System/Panic.hh>

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

namespace Cell::System {

void Log(const String& message, ...) {
    CELL_ASSERT(message.GetSize() < 1025);

    va_list parameters;
    va_start(parameters, message.ToCharPointer());

    char buffer[1025] = { 0 };
    vsnprintf(buffer, 1024, message.ToCharPointer(), parameters);

    printf("LogMessage: %s\n", buffer);

    va_end(parameters);
}

}
