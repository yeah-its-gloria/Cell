// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/DataManagement/Foreign/HTTP.hh>
#include <Cell/System/Panic.hh>

namespace Cell::DataManagement::Foreign {

Wrapped<HTTPSocket*, Result> HTTPSocket::Connect(const System::String& url) {
    (void)(url);

    CELL_UNIMPLEMENTED
}

HTTPSocket::~HTTPSocket() {
    delete this->socket;
}

Result HTTPSocket::Send() {
    CELL_UNIMPLEMENTED
}

void* HTTPSocket::Receive() {
    CELL_UNIMPLEMENTED
}

}
