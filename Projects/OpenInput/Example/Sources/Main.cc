// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Entry.hh>
#include <Cell/System/Panic.hh>
#include <openinput/openinput.h>

using namespace Cell;

void CellEntry(Reference<System::IPlatform> platform, Reference<System::String> parameterString) {
    (void)(parameterString);

    const OiApplicationInfo application_info = {
        .applicationName = "OpenInput Example",
        .applicationVersion = 1,

        .engineName = "Cell",
        .engineVersion = 0,

        .apiVersion = OI_CURRENT_API_VERSION};

    const OiInstanceCreateInfo create_info = {
        .type = OI_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .next = nullptr,

        .applicationInfo = application_info,

        .enabledExtensionCount = 0,
        .enabledExtensionNames = nullptr};

    OiInstance instance = nullptr;
    OiResult result = oiCreateInstance(create_info, &instance);
    CELL_ASSERT(result == OI_SUCCESS);

    while (platform.Unwrap().IsStillActive()) {
        result = oiAcquireUpdates(instance);
        CELL_ASSERT(result == OI_SUCCESS);

        System::Thread::Yield();
    }

    result = oiDestroyInstance(instance);
    CELL_ASSERT(result == OI_SUCCESS);
}
