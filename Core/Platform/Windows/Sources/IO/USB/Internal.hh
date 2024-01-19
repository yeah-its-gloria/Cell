// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>
#include <Cell/System/Platform/Windows/Includes.h>

#include <winusb.h>

struct usbData {
    HANDLE device;
    WINUSB_INTERFACE_HANDLE handle;
};
