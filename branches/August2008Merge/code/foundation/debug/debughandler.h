#pragma once
#ifndef DEBUG_DEBUGHANDLER_H
#define DEBUG_DEBUGHANDLER_H
//------------------------------------------------------------------------------
/**
    @class Debug::DebugHandler
    
    The message handler for the debug interface. Just wakes up from time
    to time to check for incoming Http requests.
    
    (C) 2008 Radon Labs GmbH
*/
#include "messaging/handler.h"
#include "io/console.h"
#include "debug/debugserver.h"

#if __NEBULA3_HTTP__ 
#include "http/httpserverproxy.h"
#endif
//------------------------------------------------------------------------------
namespace Debug
{
class DebugHandler : public Messaging::Handler
{
    DeclareClass(DebugHandler);
public:
    /// constructor
    DebugHandler();
    /// destructor
    virtual ~DebugHandler();
    
    /// open the handler
    virtual void Open();
    /// close the handler
    virtual void Close();
    /// do per-frame work
    virtual void DoWork();

private:
    Ptr<IO::Console> ioConsole;
    Ptr<DebugServer> debugServer;
#if __NEBULA3_HTTP__ 
    Ptr<Http::HttpServerProxy> httpServerProxy;
#endif
};

} // namespace Debug
//------------------------------------------------------------------------------
#endif
    