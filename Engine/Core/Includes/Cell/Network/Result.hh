// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::Network {

// Various network related status codes.
enum class Result : uint8_t {
    // The operation completed successfully.
    Success,

    // The given parameters were malformed.
    InvalidParameters,

    // The network is currently unreachable; e.g. no LAN cable present or Wi-Fi disconnected.
    NetworkUnavailable,

    // The underlying platform/system/subsystem is not yet ready.
    PlatformNotReadyYet,

    // Failed to discover information. This is generally temporary and requires mere waiting.
    ResolutionFailure,

    // The host could not be resolved.
    HostNotFound,

    // The port (service) of the host could not be resolved.
    PortNotFound,

    // The socket is not connected.
    NotConnected,

    // The socket lost connection.
    LostConnection,

    // The underlying runtime ran out of memory.
    OutOfMemory,

    // The message being sent was too large.
    ContentTooLarge,

    // The requested index was too large.
    OutOfRange
};

}
