//------------------------------------------------------------------------------
//  remoteinterface.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "remote/remoteinterface.h"
#include "remote/remotemessagehandler.h"

namespace Remote
{
__ImplementClass(Remote::RemoteInterface, 'REIF', Messaging::AsyncPort);
__ImplementInterfaceSingleton(Remote::RemoteInterface);

//------------------------------------------------------------------------------
/**
*/
RemoteInterface::RemoteInterface()
{
    __ConstructSingleton;
    this->SetThreadCpuCoreId(System::Cpu::MiscThreadCore);
}

//------------------------------------------------------------------------------
/**
*/
RemoteInterface::~RemoteInterface()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
RemoteInterface::Open()
{
    this->SetBehaviour(DoNotWait);
    this->SetName("RemoteInterface Thread");
    AsyncPort::Open();
}

//------------------------------------------------------------------------------
/**
*/
void
RemoteInterface::OnCreateHandlers()
{
    Ptr<RemoteMessageHandler> msgHandler = RemoteMessageHandler::Create();
    this->AttachHandler(msgHandler.upcast<Messaging::Handler>());
}

} // namespace Remote