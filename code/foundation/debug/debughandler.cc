//------------------------------------------------------------------------------
//  debughandler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "debug/debughandler.h"
#if __NEBULA3_HTTP__ 
#include "debug/debugpagehandler.h"
#endif

namespace Debug
{
ImplementClass(Debug::DebugHandler, 'DBGH', Messaging::Handler);

using namespace Util;
#if __NEBULA3_HTTP__ 
using namespace Http;
#endif
//------------------------------------------------------------------------------
/**
*/
DebugHandler::DebugHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DebugHandler::~DebugHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This method runs in the Debug thread's context and sets up the required
    runtime.
*/
void
DebugHandler::Open()
{
    n_assert(!this->IsOpen());
    Handler::Open();

    // setup core runtime, debug server and a http server proxy
    this->ioConsole = IO::Console::Create();
    this->ioConsole->Open();
    this->debugServer = DebugServer::Create();
    this->debugServer->Open();

#if __NEBULA3_HTTP__ 
    this->httpServerProxy = HttpServerProxy::Create();
    this->httpServerProxy->Open();
    this->httpServerProxy->AttachRequestHandler(Debug::DebugPageHandler::Create());
#endif
}

//------------------------------------------------------------------------------
/**
*/
void
DebugHandler::Close()
{
    n_assert(this->IsOpen());

#if __NEBULA3_HTTP__    
    this->httpServerProxy->Close();
    this->httpServerProxy = 0;
#endif    
    this->debugServer->Close();
    this->debugServer = 0;
    this->ioConsole->Close();
    this->ioConsole = 0;

    Handler::Close();
}

//------------------------------------------------------------------------------
/**
    The per-frame method just checks periodically whether there are
    any pending HttpRequests to process...
*/
void
DebugHandler::DoWork()
{
    n_assert(this->IsOpen());

#if __NEBULA3_HTTP__ 
    this->httpServerProxy->HandlePendingRequests();
#endif
    n_sleep(0.1);
}

} // namespace Debug
