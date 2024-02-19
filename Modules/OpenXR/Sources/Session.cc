// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/OpenXR/Session.hh>

namespace Cell::OpenXR {

Session::~Session() {
    xrDestroySpace(this->space);
    xrDestroySession(this->session);
}

}
