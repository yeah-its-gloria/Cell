// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/DataManagement/HTTP.hh>

namespace Cell::DataManagement::HTTP {

Wrapped<Connection*, Result> Connection::Connect(const String& url) {
    (void)(url);

    CELL_UNIMPLEMENTED
}

Connection::~Connection() {
    delete this->socket;
}

Result Connection::Send() {
    CELL_UNIMPLEMENTED
}

void* Connection::Receive() {
    CELL_UNIMPLEMENTED
}

}
