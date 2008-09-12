#pragma once
#ifndef DEBUG_COREPAGEHANDLER_H
#define DEBUG_COREPAGEHANDLER_H
//------------------------------------------------------------------------------
/**
    @class Debug::CorePageHandler
  
    Provide information about Core subsystem to debug http server.
    
    (C) 2007 Radon Labs GmbH
*/
#include "http/httprequesthandler.h"

//------------------------------------------------------------------------------
namespace Debug
{
class CorePageHandler : public Http::HttpRequestHandler
{
    DeclareClass(CorePageHandler);
public:
    /// constructor
    CorePageHandler();
    /// handle a http request, the handler is expected to fill the content stream with response data
    virtual void HandleRequest(const Ptr<Http::HttpRequest>& request);        
};

} // namespace Debug
//------------------------------------------------------------------------------
#endif
