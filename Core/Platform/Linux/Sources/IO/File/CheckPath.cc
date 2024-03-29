// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/IO/File.hh>

#include <errno.h>
#include <sys/stat.h>

namespace Cell::IO {

Result CheckPath(const String& path) {
    if (path.IsEmpty()) {
        return Result::InvalidParameters;
    }

    ScopedBlock pathCChar = path.ToCharPointer();

    struct stat buffer { };
    const int result = stat(&pathCChar, &buffer);
    if (result == -1) {
        switch (errno) {
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
            System::Panic("stat failed");
        }
        }
    }

    return Result::Success;
}

}
