// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <openinput/openinput.h>
#include <Cell/System/String.hh>

using namespace Cell::System;

PFN_oiVoidFunction oiGetInstanceProcAddr(OiInstance instance, const char* name) {
    (void)(instance);

    String nameStr = name;
    if (nameStr == "oiCreateInstance") {
        return (PFN_oiVoidFunction)oiCreateInstance;
    } else if (nameStr == "oiDestroyInstance") {
        return (PFN_oiVoidFunction)oiDestroyInstance;
    } else if (nameStr == "oiRequestUsers") {
        return (PFN_oiVoidFunction)oiRequestUsers;
    } else if (nameStr == "oiAcquireUpdates") {
        return (PFN_oiVoidFunction)oiAcquireUpdates;
    } else if (nameStr == "oiEnumerateDevices") {
        return (PFN_oiVoidFunction)oiEnumerateDevices;
    } else if (nameStr == "oiGetDeviceInfo") {
        return (PFN_oiVoidFunction)oiGetDeviceInfo;
    }

    return nullptr;
}
