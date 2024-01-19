// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Panic.hh>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace Cell::System {

void Panic(const char* message, ...) {
    if (strlen(message) > 1024) {
        abort();
    }

    va_list parameters;
    va_start(parameters, message);

    char buffer[1025] = { 0 };
    vsnprintf(buffer, 1024, message, parameters);

    fflush(stderr);
    fprintf(stderr, "%s\n", buffer);

    va_end(parameters);

    abort();
}

}
