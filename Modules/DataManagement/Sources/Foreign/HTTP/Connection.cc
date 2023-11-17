// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/DataManagement/Foreign/HTTP.hh>
#include <Cell/System/Panic.hh>

namespace Cell::DataManagement::Foreign::HTTP {

Wrapped<Connection*, Result> Connection::Connect(const System::String& url) {
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
