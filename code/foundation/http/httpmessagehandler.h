#pragma once
#ifndef HTTP_HTTPMESSAGEHANDLER_H
#define HTTP_HTTPMESSAGEHANDLER_H
//------------------------------------------------------------------------------
/**
    @class Http::HttpMessageHandler
    
    Runs the HttpServer thread, and owns the central http server. Processes
    messages sent to the HttpInterface from other threads.

    (C) 2007 Radon Labs GmbH
*/
#include "messaging/handler.h"
#include "messaging/message.h"
#include "io/console.h"
#include "http/httpserver.h"
#include "http/httpprotocol.h"

//------------------------------------------------------------------------------
namespace Http
{
class HttpMessageHandler : public Messaging::Handler
{
    __DeclareClass(HttpMessageHandler);
public:
    /// constructor
    HttpMessageHandler();
    /// destructor
    virtual ~HttpMessageHandler();

    /// open the handler
    virtual void Open();
    /// close the handler
    virtual void Close();
    /// handle a message, return true if handled
    virtual bool HandleMessage(const Ptr<Messaging::Message>& msg);
    /// do per-frame work
    virtual void DoWork();
    /// set default tcp port of http server
    static void SetDefaultTcpPort(ushort port);

private:
    /// handle AttachRequestHandler message
    void OnAttachRequestHandler(const Ptr<AttachRequestHandler>& msg);
    /// handle RemoveRequestHandler message
    void OnRemoveRequestHandler(const Ptr<RemoveRequestHandler>& msg);

    static ushort defaultTcpPort;
    Ptr<IO::Console> ioConsole;
    Ptr<HttpServer> httpServer;
};

} // namespace HttpMessageHandler
//------------------------------------------------------------------------------
#endif
