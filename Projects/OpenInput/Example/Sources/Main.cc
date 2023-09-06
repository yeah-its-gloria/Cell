// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Entry.hh>
#include <Cell/System/Panic.hh>
#include <Cell/System/Thread.hh>
#include <openinput/openinput.h>

using namespace Cell;

void CellEntry(Reference<System::String> parameterString) {
    (void)(parameterString);

    const OiApplicationInfo appInfo = {
        .applicationName = "OpenInput Example",
        .applicationVersion = 1,

        .engineName = "Cell",
        .engineVersion = 0,

        .apiVersion = OI_CURRENT_API_VERSION
    };

    const OiInstanceCreateInfo instanceInfo = {
        .type = OI_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .next = nullptr,

        .applicationInfo = appInfo,

        .enabledExtensionCount = 0,
        .enabledExtensionNames = nullptr
    };

    OiInstance instance = nullptr;
    OiResult result = oiCreateInstance(instanceInfo, &instance);
    CELL_ASSERT(result == OI_SUCCESS);

    uint32_t count = 0;
    result = oiEnumerateDevices(instance, &count, nullptr);
    CELL_ASSERT(result == OI_SUCCESS);

    while (true) {
        result = oiAcquireUpdates(instance);
        CELL_ASSERT(result == OI_SUCCESS);

        System::Thread::Yield();
    }

    result = oiDestroyInstance(instance);
    CELL_ASSERT(result == OI_SUCCESS);
}
