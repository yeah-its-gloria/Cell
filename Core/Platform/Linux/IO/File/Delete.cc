// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/IO/File.hh>

#include <errno.h>
#include <stdio.h>

namespace Cell::IO {

Result Delete(const String& path) {
    if (path.IsEmpty()) {
        return Result::InvalidParameters;
    }

    ScopedBlock pathCChar = path.ToCharPointer();
    const int result = remove(&pathCChar);
    switch (result) {
    case 0: {
        break;
    }

    case EACCES: {
        return Result::AccessDenied;
    }

    case ENOENT: {
        return Result::NotFound;
    }

    default: {
        System::Panic("remove failed");
    }
    }

    return Result::Success;
}

}
