// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/DataManagement/HTTP.hh>

namespace Cell::DataManagement::HTTP {

Wrapped<Request*, Result> Request::New() {
    return new Request(nullptr);
}

Wrapped<Request*, Result> Request::FromRaw(const Memory::IBlock& block) {
    (void)(block);

    return new Request(nullptr);
}

Request::~Request() {
    (void)(this->data);
}

}
