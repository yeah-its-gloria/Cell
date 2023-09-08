// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Network/Socket.hh>
#include <Cell/DataManagement/Result.hh>

namespace Cell::DataManagement::Foreign {

class HTTPSocket : public Object {
public:
    // Connects to an HTTP service; URL format is http://domain.com:80/stuff?whatever=this
    static Wrapped<HTTPSocket*, Result> Connect(const System::String& url);

    // Disconnects and destructs the HTTP socket.
    ~HTTPSocket();

    // Sends a request.
    Result Send();

    // Receives a response.
    void* Receive();

private:
    HTTPSocket(Network::Socket* socket) : socket(socket) { }

    Network::Socket* socket;
};

// Parses an HTTP response.
CELL_FUNCTION Result HTTPParseResponse(const uint8_t* CELL_NONNULL data, const size_t size);

}
