// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/DataManagement/Result.hh>
#include <Cell/Network/Socket.hh>

namespace Cell::DataManagement::HTTP {

// Methods for requesting content.
enum class Method : uint8_t {
    // Retrieves content.
    GET,

    // Requests content, but the server only sends headers.
    HEAD,

    // Creates or updates content.
    POST,

    // Creates content.
    PUT,

    // Deletes content.
    DELETE,

    // Used for tunnels.
    CONNECT,

    // Checks or changes options for a desired resource.
    OPTIONS,

    // Loops messages back for a connection test.
    TRACE
};

// Various status codes for responses.
enum class StatusCode : uint16_t {
    // The server has received the request, but not yet processed it.
    Continue = 100,

    // The server negotiated a new protocol for us, e.g HTTP 2.0/3.0 or WebSocket.
    SwitchingProtocols = 101,

    // Indicates a successful request.
    OK = 200,

    // A new resource was created.
    Created = 201,

    // The request was accepted by the server, but has not yet finished processing.
    Accepted = 202,

    // A proxy modified the contents given.
    NonAuthoritativeInformation = 203,

    // No content was sent.
    NoContent = 204,

    // Indicates the content was cleared fully, and the client should refresh the document in its entirety.
    ResetContent = 205,

    // Indicates that only a part of the full content has been sent.
    PartialContent = 206,

    // The resource desired is not unique and is instead available at different locations, with the client being given choices. Check the Location header entry.
    MultipleChoices = 300,

    // The given resource was moved permanently. Check the Location header entry.
    MovedPermanently = 301,

    // The given resource was moved temporarily. Check the Location header entry. Unlike TemporaryRedirect (307), this code allows switching methods from POST to GET.
    Found = 302,

    // The client is being redirected to another resource at the server's behest. This serves as an indirect redirection. Check the Location header entry.
    SeeOther = 303,

    // The resource requested has not yet changed.
    NotModified = 304,

    // Deprecated. Indicated that a proxy was needed.
    UseProxy = 305,

    // The given resource was moved temporarily. Check the Location header entry.
    TemporaryRedirect = 307,

    // The given resource was moved permanently. Check the Location header entry.
    PermanentRedirect = 308,

    // The request contents were invalid.
    BadRequest = 400,

    // The client has to be authorized to perform this request.
    Unauthorized = 401,

    // Reserved for future use. God help us all.
    PaymentRequired = 402,

    // The client cannot access this resource.
    Forbidden = 403,

    // The resource could not be found.
    NotFound = 404,

    // The resource cannot be accessed with this method.
    MethodNotAllowed = 405,

    // The resource cannot be represented in a fashion the client desires.
    NotAcceptable = 406,

    // The client needs to be authorized to route this request through a proxy.
    ProxyAuthenticationRequired = 407,

    // The client took too long to submit this request.
    RequestTimeout = 408,

    // The current resource is encountering a conflict.
    Conflict = 409,

    // The resource has been removed, likely permanently.
    Gone = 410,

    // The server requires this request to contain the Content-Length field.
    LengthRequired = 411,

    // A condition provided by the client could not be met.
    PreconditionFailed = 412,

    // The content of the request was too large to be processed.
    ContentTooLarge = 413,

    // The URI was too large to be processed.
    URITooLong = 414,

    // The content sent by the client cannot be accepted by the server.
    UnsupportedMediaType = 415,

    // The ranges in the Range header field were unsatisfiable.
    RangeNotSatisfiable = 416,

    // An expectation in the Expect header field was not met.
    ExpectationFailed = 417,

    // What do I say here?
    Teapot = 418,

    // The request was directed at a server that could not respond appropriately.
    MisdirectedRequest = 421,

    // The server understood the type of content sent, but cannot process it due to other issues.
    UnprocessableContent = 422,

    // A connection upgrade (e.g HTTP/3.0) is necessary to continue. Check the Upgrade header field.
    UpgradeRequired = 426,

    // An unspecified server failure occurred.
    InternalServerError = 500,

    // The server does not implement the functionality necessary.
    NotImplemented = 501,

    // A gateway or proxy received a malformed response from an acting server.
    BadGateway = 502,

    // The server is not capable of serving the needed functionality at the moment. May provide a Retry-After header field with a date as an indication of when to try again.
    ServiceUnavailable = 503,

    // The gateway server did not receive a response from an acting server for too long.
    GatewayTimeout = 504,

    // The server does not support the desired HTTP version.
    VersionNotSupported = 505
};

// Represents an HTTP connection.
class Connection : public Object {
public:
    // Connects to an HTTP service; Host has to be formed as whatever.the.domain.name.is
    CELL_FUNCTION static Wrapped<Connection*, Result> Connect(const String& host);

    // Disconnects and destructs the HTTP socket.
    CELL_FUNCTION ~Connection();

    // Sends a request.
    CELL_FUNCTION Result Send();

    // Receives a response.
    CELL_FUNCTION void* Receive();

private:
    CELL_FUNCTION_INTERNAL Connection(Network::Socket* s) : socket(s) { }

    Network::Socket* socket;
};

class Request : public Object {
public:
    CELL_FUNCTION static Wrapped<Request*, Result> New();
    CELL_FUNCTION static Wrapped<Request*, Result> FromRaw(const Memory::IBlock& block);

    CELL_FUNCTION ~Request();

private:
    CELL_FUNCTION_INTERNAL Request(uint8_t* d) : data(d) { }

    uint8_t* data;
};

}
