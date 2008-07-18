#pragma once
#ifndef HTTP_HTTPSERVER_H
#define HTTP_HTTPSERVER_H
//------------------------------------------------------------------------------
/**
    @class Http::HttpServer
    
    Implements an extremly simple standalone HTTP server with attached
    HttpRequestHandlers. Can be used to serve debug information about the 
    Nebula3 application to web browsers.

    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "net/tcpserver.h"
#include "net/ipaddress.h"
#include "io/textreader.h"
#include "io/textwriter.h"
#include "http/httpresponsewriter.h"
#include "http/httprequesthandler.h"
#include "http/defaulthttprequesthandler.h"

//------------------------------------------------------------------------------
namespace Http
{
class HttpServer : public Core::RefCounted
{
    DeclareClass(HttpServer);
    DeclareSingleton(HttpServer);
public:
    /// constructor
    HttpServer();
    /// destructor
    virtual ~HttpServer();
    
    /// set port number for http service
    void SetPort(ushort p);
    /// get port number of http service
    ushort GetPort() const;
    /// open the http server
    bool Open();
    /// close the http server
    void Close();
    /// return true if server is open
    bool IsOpen() const;
    /// attach a request handler to the server
    void AttachRequestHandler(const Ptr<HttpRequestHandler>& h);
    /// remove a request handler from the server
    void RemoveRequestHandler(const Ptr<HttpRequestHandler>& h);
    /// get array of currently attached request handlers
    const Util::Array<Ptr<HttpRequestHandler> >& GetRequestHandlers() const;
    /// call this method frequently to serve http connections
    void OnFrame();

private:
    /// handle a http request
    bool HandleHttpRequest(const Ptr<IO::Stream>& recvStream, const Ptr<IO::Stream>& sendStream);

    Ptr<DefaultHttpRequestHandler> defaultRequestHandler;
    Util::Array<Ptr<HttpRequestHandler> > requestHandlers;
    Net::IpAddress ipAddress;
    Ptr<Net::TcpServer> tcpServer;
    bool isOpen;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
HttpServer::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
inline void
HttpServer::SetPort(ushort p)
{
    this->ipAddress.SetPort(p);
}

//------------------------------------------------------------------------------
/**
*/
inline ushort
HttpServer::GetPort() const
{
    return this->ipAddress.GetPort();
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<HttpRequestHandler> >&
HttpServer::GetRequestHandlers() const
{
    return this->requestHandlers;
}

} // namespace Http
//------------------------------------------------------------------------------
#endif
    