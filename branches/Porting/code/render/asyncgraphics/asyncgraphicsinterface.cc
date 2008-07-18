//------------------------------------------------------------------------------
//  asyncgraphicsinterface.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "asyncgraphics/asyncgraphicsinterface.h"
#include "asyncgraphics/asyncgraphicshandler.h"
#include "threading/thread.h"

namespace AsyncGraphics
{
ImplementClass(AsyncGraphics::AsyncGraphicsInterface, 'AGIF', Messaging::AsyncPort);
ImplementInterfaceSingleton(AsyncGraphics::AsyncGraphicsInterface);

//------------------------------------------------------------------------------
/**
*/
AsyncGraphicsInterface::AsyncGraphicsInterface()
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
AsyncGraphicsInterface::~AsyncGraphicsInterface()
{
    DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
AsyncGraphicsInterface::Open()
{
    // we need to run continously, not wait for messages
    this->SetWaitForMessages(false);
    this->SetName("AsyncGraphicsInterface");
    AsyncPort::Open();
}

//------------------------------------------------------------------------------
/**
*/
void
AsyncGraphicsInterface::OnCreateHandlers()
{
    Ptr<AsyncGraphicsHandler> msgHandler = AsyncGraphicsHandler::Create();
    this->AttachHandler(msgHandler.upcast<Messaging::Handler>());
}

} // namespace AsyncGraphics
