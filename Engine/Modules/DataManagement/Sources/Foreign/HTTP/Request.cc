// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/DataManagement/Foreign/HTTP.hh>
#include <Cell/System/Panic.hh>

namespace Cell::DataManagement::Foreign::HTTP {

Wrapped<Request*, Result> Request::New() {
    return new Request(nullptr);
}

Wrapped<Request*, Result> Request::FromRaw(const IBlock& block) {
    (void)(block);

    return new Request(nullptr);
}

Request::~Request() {
    (void)(this->data);
}

}
