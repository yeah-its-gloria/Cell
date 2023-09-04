// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/IO/File.hh>

#include <errno.h>
#include <unistd.h>

namespace Cell::IO {

Result File::SetWorkingDirectory(const System::String& path) {
    if (path.IsEmpty()) {
        return Result::InvalidParameters;
    }

    ScopedBlock<char> pathCChar = path.ToCharPointer();
    const int result = chdir(&pathCChar);
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
        System::Panic("chdir failed");
    }
    }

    return Result::Success;
}

}
