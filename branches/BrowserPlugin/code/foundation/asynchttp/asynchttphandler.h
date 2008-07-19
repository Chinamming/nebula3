#pragma once
#ifndef ASYNCHTTP_ASYNCHTTPHANDLER_H
#define ASYNCHTTP_ASYNCHTTPHANDLER_H
//------------------------------------------------------------------------------
/**
    @class AsyncHttp::AsyncHttpHandler
    
    Runs the AsyncHttp thread, and processes messages to the Http
    subsystem.

    (C) 2007 Radon Labs GmbH
*/
#include "messaging/handler.h"
#include "messaging/message.h"

//------------------------------------------------------------------------------
namespace AsyncHttp
{
class AsyncHttpHandler : public Messaging::Handler
{
    DeclareClass(AsyncHttpHandler);
public:
    /// constructor
    AsyncHttpHandler();
    /// destructor
    virtual ~AsyncHttpHandler();

    /// open the handler
    virtual void Open();
    /// close the handler
    virtual void Close();
    /// handle a message, return true if handled
    virtual bool HandleMessage(const Ptr<Messaging::Message>& msg);
};
//------------------------------------------------------------------------------
#endif
