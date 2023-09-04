// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Input.hh>
#include <Cell/System/Platform/Windows.hh>

namespace Cell::Shell
{

Result Input::RegisterAction(const KeyValue value, const InputFunction function, void* userData)
{
    System::Platform::Windows* windows = (System::Platform::Windows*)&this->platform;
    if (windows->window == nullptr)
    {
        return Result::PlatformNotReadyYet;
    }

    const registerInfo info =
    {
        .match    = value,
        .function = function,
        .userData = userData
    };

    this->registeredFunctions.Append(info);

    return Result::Success;
}

Result Input::Poll()
{
    System::Platform::Windows* windows = (System::Platform::Windows*)&this->platform;
    if (windows->window == nullptr)
    {
        return Result::PlatformNotReadyYet;
    }

    windows->keyLock->Lock();

    for (registerInfo info : this->registeredFunctions)
    {
        if ((windows->keys & info.match) == info.match)
        {
            info.function(info.match, info.userData);
        }
    }

    windows->keyLock->Unlock();

    return Result::Success;
}


}
