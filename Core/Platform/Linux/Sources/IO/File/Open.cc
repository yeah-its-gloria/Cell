// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/IO/File.hh>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

namespace Cell::IO {

#define HAS_MODE(in) ((FileMode::in & mode) == FileMode::in)

Wrapped<File*, Result> File::Open(const String& path, const FileMode mode) {
    if (path.IsEmpty()) {
        return Result::InvalidParameters;
    }

    int flags = O_LARGEFILE;
    if (HAS_MODE(Read)) {
        flags |= O_RDONLY;
    }

    if (HAS_MODE(Write)) {
        flags |= O_WRONLY;
    }

    if (HAS_MODE(Create)) {
        if (HAS_MODE(Overwrite) || HAS_MODE(Open)) {
            return Result::InvalidParameters;
        }

        flags |= O_CREAT | O_EXCL;
    } else if (HAS_MODE(Overwrite)) {
        if (HAS_MODE(Open)) {
            return Result::InvalidParameters;
        }

        flags |= O_TRUNC;
    } else if (!HAS_MODE(Open) || (!HAS_MODE(Read) && !HAS_MODE(Write))) {
        return Result::InvalidParameters;
    }

    ScopedBlock pathStr = path.ToCharPointer();
    const int descriptor = open(&pathStr, flags);
    if (descriptor == -1) {
        switch (errno) {
        case 0: {
            break;
        }

        case EACCES: {
            return Result::AccessDenied;
        }

        case EDQUOT: {
            return Result::InsufficientStorage;
        }

        case ENOENT: {
            return Result::NotFound;
        }

        case EBUSY: {
            return Result::Locked;
        }

        case EEXIST: {
            return Result::AlreadyExists;
        }

        case ENAMETOOLONG: {
            return Result::InvalidParameters;
        }

        case ENOMEM: {
            return Result::NotEnoughMemory;
        }

        default: {
            System::Panic("open failed");
        }
        }
    }

    char descMode[4] = { 0, 0, 0, 0 };
    if (HAS_MODE(Read)) {
        strcat(descMode, "r");
    }

    if (HAS_MODE(Write)) {
        strcat(descMode, "w");
    }

    strcat(descMode, "b");

    FILE* _file = fdopen(descriptor, descMode);
    CELL_ASSERT(_file != nullptr);

    return new File((uintptr_t)_file);
}

}
