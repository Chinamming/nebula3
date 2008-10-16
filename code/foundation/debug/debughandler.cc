//------------------------------------------------------------------------------
//  debughandler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "debug/debughandler.h"
#include "debug/debugpagehandler.h"
#include "timing/time.h"

namespace Debug
{
__ImplementClass(Debug::DebugHandler, 'DBGH', Messaging::Handler);

using namespace Util;
using namespace Http;

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

    this->httpServerProxy = HttpServerProxy::Create();
    this->httpServerProxy->Open();
    this->httpServerProxy->AttachRequestHandler(Debug::DebugPageHandler::Create());
}

//------------------------------------------------------------------------------
/**
*/
void
DebugHandler::Close()
{
    n_assert(this->IsOpen());

    this->httpServerProxy->Close();
    this->httpServerProxy = 0;

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
    this->httpServerProxy->HandlePendingRequests();
    Timing::Sleep(0.1);
}

} // namespace Debug
