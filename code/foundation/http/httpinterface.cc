//------------------------------------------------------------------------------
//  httpinterface.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "http/httpinterface.h"
#include "http/httpmessagehandler.h"

namespace Http
{
ImplementClass(Http::HttpInterface, 'HTIF', Messaging::AsyncPort);
ImplementInterfaceSingleton(Http::HttpInterface);

//------------------------------------------------------------------------------
/**
*/
HttpInterface::HttpInterface()
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
HttpInterface::~HttpInterface()
{
    DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
HttpInterface::Open()
{
    this->SetWaitForMessages(false);
    this->SetName("HttpInterface");
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