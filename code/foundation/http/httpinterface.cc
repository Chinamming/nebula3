//------------------------------------------------------------------------------
//  httpinterface.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "http/httpinterface.h"
#include "http/httpmessagehandler.h"

namespace Http
{
__ImplementClass(Http::HttpInterface, 'HTIF', Messaging::AsyncPort);
__ImplementInterfaceSingleton(Http::HttpInterface);

//------------------------------------------------------------------------------
/**
*/
HttpInterface::HttpInterface()
{
    __ConstructInterfaceSingleton;
    this->SetThreadCpuCoreId(System::Cpu::MiscThreadCore);
}

//------------------------------------------------------------------------------
/**
*/
HttpInterface::~HttpInterface()
{
    __DestructInterfaceSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
HttpInterface::Open()
{
    this->SetBehaviour(DoNotWait);
    this->SetName("HttpInterface Thread");
    AsyncPort::Open();
}

//------------------------------------------------------------------------------
/**
*/
void
HttpInterface::OnCreateHandlers()
{
    Ptr<HttpMessageHandler> msgHandler = HttpMessageHandler::Create();
    this->AttachHandler(msgHandler.upcast<Messaging::Handler>());
}

} // namespace Http
