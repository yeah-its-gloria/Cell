// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Window.hh>
#include <Cell/System/Panic.hh>
#include <Cell/System/Platform/Windows.hh>

namespace Cell::Shell
{

Wrapped<Extent, Result> GetDrawableExtentForWindow(System::IPlatform& platform)
{
    System::Platform::Windows* windows = (System::Platform::Windows*)&platform;
    if (windows->GetWindow() == nullptr)
    {
        return Result::InvalidPlatform;
    }

    RECT rect = { 0, 0, 0, 0 };
    const BOOL result = GetClientRect(windows->GetWindow(), &rect);
    if (result == FALSE)
    {
        switch (GetLastError())
        {
        default:
        {
            System::Panic("GetClientRect failed");
        }
        }
    }

    return Extent { (uint32_t)rect.right - rect.left, (uint32_t)rect.bottom - rect.top };
}

Result SetDrawableExtentForWindow(System::IPlatform& platform, const Extent extent)
{
    System::Platform::Windows* windows = (System::Platform::Windows*)&platform;
    if (windows->GetWindow() == nullptr)
    {
        return Result::InvalidPlatform;
    }

    const BOOL result = SetWindowPos(windows->GetWindow(), nullptr, 0, 0, extent.width, extent.height, SWP_NOMOVE);
    if (result == FALSE)
    {
        switch (GetLastError())
        {
        default:
        {
            System::Panic("SetWindowPos failed");
        }
        }
    }

    return Result::Success;
}

}
