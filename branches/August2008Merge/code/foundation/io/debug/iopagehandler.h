#pragma once
#ifndef DEBUG_IOPAGEHANDLER_H
#define DEBUG_IOPAGEHANDLER_H
//------------------------------------------------------------------------------
/**
    @class Debug::IoPageHandler
    
    Provide information about IO subsystem to debug http server.
    
    (C) 2007 Radon Labs GmbH
*/
#include "http/httprequesthandler.h"

//------------------------------------------------------------------------------
namespace Debug
{
class IoPageHandler : public Http::HttpRequestHandler
{
    DeclareClass(IoPageHandler);
public:
    /// constructor
    IoPageHandler();
    /// return true if the http request is accepted by the request handler
    virtual bool AcceptsRequest(const Ptr<Http::HttpRequest>& request);
    /// handle a http request, the handler is expected to fill the content stream with response data
    virtual void HandleRequest(const Ptr<Http::HttpRequest>& request); 

private:
    /// create a directory lister page
    void ListDirectory(const Util::String& path, const Ptr<Http::HttpRequest>& request);
};

} // namespace Debug
//------------------------------------------------------------------------------
#endif
    