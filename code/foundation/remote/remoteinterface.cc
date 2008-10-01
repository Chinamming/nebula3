//------------------------------------------------------------------------------
//  remoteinterface.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "remote/remoteinterface.h"
#include "remote/remotemessagehandler.h"

namespace Remote
{
ImplementClass(Remote::RemoteInterface, 'REIF', Messaging::AsyncPort);
ImplementInterfaceSingleton(Remote::RemoteInterface);

//------------------------------------------------------------------------------
/**
*/
RemoteInterface::RemoteInterface()
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
RemoteInterface::~RemoteInterface()
{
    DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
RemoteInterface::Open()
{
    this->SetWaitForMessages(false);
    this->SetName("RemoteInterface");
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