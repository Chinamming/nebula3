//------------------------------------------------------------------------------
//  debuginterface.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "debug/debuginterface.h"
#include "debug/debughandler.h"

namespace Debug
{
__ImplementClass(Debug::DebugInterface, 'DBIF', Messaging::AsyncPort);
__ImplementInterfaceSingleton(Debug::DebugInterface);

//------------------------------------------------------------------------------
/**
*/
DebugInterface::DebugInterface()
{
    __ConstructInterfaceSingleton;
    this->SetThreadCpuCoreId(System::Cpu::MiscThreadCore);
}

//------------------------------------------------------------------------------
/**
*/
DebugInterface::~DebugInterface()
{
    __DestructInterfaceSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
DebugInterface::Open()
{
    this->SetBehaviour(DoNotWait);
    this->SetName("DebugInterface Thread");
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
