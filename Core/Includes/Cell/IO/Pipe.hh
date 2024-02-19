// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/String.hh>
#include <Cell/IO/Result.hh>
#include <Cell/System/Block.hh>
#include <Cell/Utilities/Preprocessor.hh>

namespace Cell::IO {

// Different modes of operation for a pipe.
enum class PipeMode : uint8_t {
    Read = 1 << 0,
    Write = 1 << 1
};

CELL_ENUM_CLASS_OPERATORS(PipeMode)

// Pipes are means of interprocess communication.
class Pipe : public Object {
public:
    // Creates a new pipe.
    CELL_FUNCTION static Wrapped<Pipe*, Result> Create(const String& name, const size_t blockSize, const PipeMode mode = (PipeMode)((uint8_t)PipeMode::Read | (uint8_t)PipeMode::Write));

    // Connects to an existing pipe.
    CELL_FUNCTION static Wrapped<Pipe*, Result> Connect(const String& name, const PipeMode mode = (PipeMode)((uint8_t)PipeMode::Read | (uint8_t)PipeMode::Write));

    // Shuts down the pipe.
    CELL_FUNCTION ~Pipe();

    // Reads count bytes of data from the pipe as one block.
    CELL_FUNCTION Result Read(IBlock& data);

    // Writes count bytes of data to the pipe as one block.
    CELL_FUNCTION Result Write(const IBlock& data);

    // Waits for a client to connect.
    CELL_FUNCTION Result WaitForClient();

    // Disconnects the current client.
    CELL_FUNCTION Result DisconnectClient();

    // Waits for the pipe to be created and ready to accept clients.
    // By default, it waits forever.
    CELL_FUNCTION static Result WaitUntilReady(const String& name, const uint32_t timeoutMilliseconds = 0);

    CELL_NON_COPYABLE(Pipe)

private:
    CELL_FUNCTION_INTERNAL CELL_INLINE Pipe(const uintptr_t handle, const bool isClient = false) : handle(handle), isClient(isClient) { }

    const uintptr_t handle;
    const bool isClient;
};

}
