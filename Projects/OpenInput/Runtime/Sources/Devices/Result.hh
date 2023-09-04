// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Runtime::Devices {

enum class Result : uint8_t {
    Success,
    NotFound,
    Disconnected,
    Timeout,
    InvalidData,
    Failure
};

}
