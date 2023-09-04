// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/Shell/Window.hh>
#include <Cell/System/Panic.hh>
#include <Cell/System/Platform/Windows.hh>

namespace Cell::Shell
{

Result SetNewTitleForWindow(System::IPlatform& platform, const System::String& title)
{
    System::Platform::Windows* windows = (System::Platform::Windows*)&platform;
    if (windows->GetWindow() == nullptr)
    {
        return Result::InvalidPlatform;
    }

    if (title.IsEmpty())
    {
        return Result::InvalidParameters;
    }

    ScopedBlock<wchar_t> titleWide = title.ToPlatformWideString();
    const BOOL result = SetWindowTextW(windows->GetWindow(), &titleWide);
    if (result == FALSE)
    {
        switch (GetLastError())
        {
        default:
        {
            System::Panic("SetWindowTextW failed");
        }
        }
    }

    return Result::Success;
}

}
