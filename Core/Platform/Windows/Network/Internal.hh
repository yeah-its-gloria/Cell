// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Wrapped.hh>
#include <Cell/Network/Properties.hh>
#include <Cell/Network/Result.hh>
#include <Cell/System/Platform/Windows/Includes.h>

#include <WinSock2.h>

namespace Cell::Network {

struct systemTypes {
    int transport;
    int type;
    int protocol;
};

CELL_FUNCTION_INTERNAL Wrapped<systemTypes, Result> convertPropertiesToSystemTypes(const Transport transport, const ConnectionType type, const Protocol protocol);

}
