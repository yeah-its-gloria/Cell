// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Panic.hh>

#include <stdio.h>
#include <stdlib.h>

namespace Cell::System {

void SoftPanic() {
    printf("An error occurred that caused the engine and/or title to crash.");

    exit(0);
}

}
