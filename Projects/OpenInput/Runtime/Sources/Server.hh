// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/System/Log.hh>
#include <Cell/System/Platform.hh>

namespace Runtime {

class Server : public Cell::Object {
public:
    CELL_INLINE Server(Cell::System::IPlatform platform) : platform(platform) {}

    CELL_INLINE ~Server() {}

    CELL_FUNCTION_INTERNAL void Launch();

private:
    CELL_FUNCTION_INTERNAL void DeviceThread();

    Cell::System::IPlatform platform;
};

}
