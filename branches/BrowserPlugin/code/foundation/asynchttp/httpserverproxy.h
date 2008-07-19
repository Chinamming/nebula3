#pragma once
#ifndef ASYNCHTTP_HTTPSERVERPROXY_H
#define ASYNCHTTP_HTTPSERVERPROXY_H
//------------------------------------------------------------------------------
/**
    @class AsyncHttp::HttpServerProxy
    
    Client-side proxy of the HttpServer. Client threads create and
    attach HttpRequestHandlers to their HttpServerProxy. The HttpServerProxy 
    receives incoming http requests from the http thread, and lets
    its HttpRequestHandlers process the request in the client thread's
    context, then sends the result back to the http thread.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"

//------------------------------------------------------------------------------
namespace AsyncHttp
{
class HttpServerProxy : public Core::RefCounted
{
    DeclareClass(HttpServerProxy);
    DeclareSingleton(HttpServerProxy);
public:
    /// constructor
    HttpServerProxy();
    /// destructor
    virtual ~HttpServerProxy();

    /// attach a request handler to the server
    void AttachRequestHandler(const Ptr<HttpRequestHandler>& h);
    /// remove a request handler from the server
    void RemoveRequestHandler(const Ptr<HttpRequestHandler>& h);
    /// call this method frequently to process pending requests
    void OnFrame();

private:

    
        

} // namespace AsyncHttp
//------------------------------------------------------------------------------
#endif
    