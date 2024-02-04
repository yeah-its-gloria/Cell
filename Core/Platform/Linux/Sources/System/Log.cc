// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/System/Log.hh>

#include <stdio.h>

namespace Cell::System {

void Log(const String& message) {
    ScopedBlock messageStr = message.ToCharPointer();
    printf("LogMessage: %s\n", &messageStr);
}

}
