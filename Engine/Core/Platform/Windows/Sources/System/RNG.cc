// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/RNG.hh>
#include <Cell/System/Platform/Windows/Includes.h>

#include <bcrypt.h>
#include <ntstatus.h>

namespace Cell::System {

Wrapped<RandomNumberGenerator*, Result> RandomNumberGenerator::New() {
    BCRYPT_ALG_HANDLE handle = nullptr;

    const NTSTATUS result = BCryptOpenAlgorithmProvider(&handle, BCRYPT_RNG_ALGORITHM, nullptr, 0);
    switch (result) {
    case STATUS_SUCCESS: {
        break;
    }

    case STATUS_NO_MEMORY: {
        return Result::OutOfMemory;
    }

    default: {
        Panic("BCryptOpenAlgorithmProvider failed");
    }
    }

    return new RandomNumberGenerator((uintptr_t)handle);
}

RandomNumberGenerator::~RandomNumberGenerator() {
    const NTSTATUS status = BCryptCloseAlgorithmProvider((BCRYPT_ALG_HANDLE)this->handle, 0);
    CELL_ASSERT(status == STATUS_SUCCESS);
}

Wrapped<uint64_t, Result> RandomNumberGenerator::Generate() {
    uint64_t data = 0;
    const NTSTATUS result = BCryptGenRandom((BCRYPT_ALG_HANDLE)this->handle, (uint8_t*)&data, sizeof(uint64_t), 0);
    switch (result) {
    case STATUS_SUCCESS: {
        break;
    }

    case STATUS_NO_MEMORY: {
        return Result::OutOfMemory;
    }

    default: {
        Panic("BCryptGenRandom failed");
    }
    }

    return data;
}

}
