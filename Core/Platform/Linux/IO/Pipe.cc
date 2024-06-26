// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/IO/File.hh>
#include <Cell/IO/Pipe.hh>

#include <errno.h>
#include <sys/stat.h>

#define HAS_MODE(in) ((PipeMode::in & mode) == PipeMode::in)

namespace Cell::IO {

Pipe::~Pipe() {
    delete (File*)this->handle;
}

Wrapped<Pipe*, Result> Pipe::Create(const String& name, const size_t blockSize, const PipeMode mode) {
    if (name.IsEmpty() || blockSize == 0 || (!HAS_MODE(Read) && !HAS_MODE(Write))) {
        return Result::InvalidParameters;
    }

    String path = String("/tmp/") + name;

    ScopedBlock<char> fullPath = path.ToCharPointer();
    const int result = mkfifo(&fullPath, 0666);
    if (result < 0) {
        switch (errno) {
        case EACCES: {
            return Result::AccessDenied;
        }

        case EDQUOT: {
            return Result::InsufficientStorage;
        }

        default: {
            System::Panic("mkfifo failed");
        }
        }
    }

    FileMode fileMode;
    if ((mode & PipeMode::Read) == PipeMode::Read) {
        fileMode |= FileMode::Read;
    }

    if ((mode & PipeMode::Write) == PipeMode::Write) {
        fileMode |= FileMode::Write;
    }

    Wrapped<File*, Result> fileResult = File::Open(path, fileMode);
    if (!fileResult.IsValid()) {
        return fileResult.Result();
    }

    return new Pipe((uintptr_t)fileResult.Unwrap());
}

Wrapped<Pipe*, Result> Pipe::Connect(const String& name, const PipeMode mode) {
    if (name.IsEmpty() || (!HAS_MODE(Read) && !HAS_MODE(Write))) {
        return Result::InvalidParameters;
    }

    FileMode fileMode;
    if ((mode & PipeMode::Read) == PipeMode::Read) {
        fileMode |= FileMode::Read;
    }

    if ((mode & PipeMode::Write) == PipeMode::Write) {
        fileMode |= FileMode::Write;
    }

    String path = String("/tmp/") + name;
    Wrapped<File*, Result> fileResult = File::Open(path, fileMode);
    if (!fileResult.IsValid()) {
        return fileResult.Result();
    }

    return new Pipe((uintptr_t)fileResult.Unwrap());
}

Result Pipe::Read(Memory::IBlock& data) {
    return ((File*)this->handle)->Read(data);
}

Result Pipe::Write(const Memory::IBlock& data) {
    return ((File*)this->handle)->Write(data);
}

Result Pipe::WaitUntilReady(const String& name, const uint32_t timeout_ms) {
    (void)(timeout_ms);

    if (name.IsEmpty()) {
        return Result::InvalidParameters;
    }

    CELL_UNIMPLEMENTED

    return Result::Success;
}

Result Pipe::WaitForClient() {
    if (this->isClient) {
        return Result::InvalidOperation;
    }

    CELL_UNIMPLEMENTED

    return Result::Success;
}

Result Pipe::DisconnectClient() {
    if (this->isClient) {
        return Result::InvalidOperation;
    }

    CELL_UNIMPLEMENTED

    return Result::Success;
}

}
