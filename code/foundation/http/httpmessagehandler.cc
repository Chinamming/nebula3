//------------------------------------------------------------------------------
//  httpmessagehandler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "http/httpmessagehandler.h"

namespace Http
{
__ImplementClass(Http::HttpMessageHandler, 'HTMH', Messaging::Handler);

using namespace Messaging;

ushort HttpMessageHandler::defaultTcpPort = 2100;

//------------------------------------------------------------------------------
/**
*/
HttpMessageHandler::HttpMessageHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
HttpMessageHandler::~HttpMessageHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
HttpMessageHandler::Open()
{   
    n_assert(!this->IsOpen());
    Handler::Open();

    // setup core runtime and central http server
    this->ioConsole = IO::Console::Create();
    this->ioConsole->Open();
    this->httpServer = HttpServer::Create();
    this->httpServer->SetPort(HttpMessageHandler::defaultTcpPort);
    this->httpServer->Open();
}

//------------------------------------------------------------------------------
/**
*/
void
HttpMessageHandler::Close()
{
    n_assert(this->IsOpen());

    this->httpServer->Close();
    this->httpServer = 0;

    this->ioConsole->Close();
    this->ioConsole = 0;

    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
    Triggers the http server from time to time.
*/
void
HttpMessageHandler::DoWork()
{
    n_assert(this->IsOpen());

    // only process http requests once in a while
    this->httpServer->OnFrame();
    n_sleep(0.1f);
}

//------------------------------------------------------------------------------
/**
*/
bool
HttpMessageHandler::HandleMessage(const Ptr<Message>& msg)
{
    n_assert(msg.isvalid());

    if (msg->CheckId(AttachRequestHandler::Id))
    {
        this->OnAttachRequestHandler(msg.cast<AttachRequestHandler>());
    }
    else if (msg->CheckId(RemoveRequestHandler::Id))
    {
        this->OnRemoveRequestHandler(msg.cast<RemoveRequestHandler>());
    }
    else
    {
        // unknown message
        return false;
    }
    // fallthrough: message was handled
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
HttpMessageHandler::OnAttachRequestHandler(const Ptr<AttachRequestHandler>& msg)
{
    n_printf("HttpMessageHandler::OnAttachRequestHandler() called!\n");
    this->httpServer->AttachRequestHandler(msg->GetRequestHandler());
}

//------------------------------------------------------------------------------
/**
*/
void
HttpMessageHandler::OnRemoveRequestHandler(const Ptr<RemoveRequestHandler>& msg)
{
    n_printf("HttpMessageHandler::OnRemoveRequestHandler() called!\n");
    this->httpServer->RemoveRequestHandler(msg->GetRequestHandler());
}

//------------------------------------------------------------------------------
/**
*/
void
HttpMessageHandler::SetDefaultTcpPort(ushort port)
{
    HttpMessageHandler::defaultTcpPort = port;
}
} // namespace Http
