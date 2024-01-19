// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Mutex.hh>
#include <Cell/System/Panic.hh>
#include <Cell/System/Platform/Windows/Includes.h>

namespace Cell::System {

Mutex::Mutex(const bool createLocked) {
    HANDLE mutex = CreateMutexW(nullptr, createLocked, nullptr);
    CELL_ASSERT(mutex != nullptr);

    this->handle = (uintptr_t)mutex;
}

Mutex::~Mutex() {
    BOOL result = ReleaseMutex((HANDLE)this->handle);
    CELL_ASSERT(result == TRUE || (result == FALSE && GetLastError() == ERROR_NOT_OWNER));

    result = CloseHandle((HANDLE)this->handle);
    CELL_ASSERT(result == TRUE);
}

void Mutex::Lock() {
    const DWORD result = WaitForSingleObjectEx((HANDLE)this->handle, INFINITE, FALSE);
    CELL_ASSERT(result == WAIT_OBJECT_0);
}

void Mutex::Unlock() {
    const BOOL result = ReleaseMutex((HANDLE)this->handle);
    CELL_ASSERT(result == TRUE);
}

}
