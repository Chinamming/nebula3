//------------------------------------------------------------------------------
//  debuginterface.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "debug/debuginterface.h"
#include "debug/debughandler.h"

namespace Debug
{
ImplementClass(Debug::DebugInterface, 'DBIF', Messaging::AsyncPort);
ImplementInterfaceSingleton(Debug::DebugInterface);

//------------------------------------------------------------------------------
/**
*/
DebugInterface::DebugInterface()
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
DebugInterface::~DebugInterface()
{
    DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
DebugInterface::Open()
{
    this->SetWaitForMessages(false);
    this->SetName("DebugInterface");
    AsyncPort::Open();
}

//------------------------------------------------------------------------------
/**
*/
void
DebugInterface::OnCreateHandlers()
{
    Ptr<DebugHandler> debugHandler = DebugHandler::Create();
    this->AttachHandler(debugHandler.upcast<Messaging::Handler>());
}

} // namespace Debug