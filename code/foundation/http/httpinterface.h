#pragma once
#ifndef HTTP_HTTPINTERFACE_H
#define HTTP_HTTPINTERFACE_H
//------------------------------------------------------------------------------
/**
    @class Http::HttpInterface
    
    The HttpInterface launches the HttpServer thread and is the communication
    interface with the HttpServer thread.
    
    (C) 2008 Radon Labs GmbH
*/
#include "messaging/asyncport.h"
#include "core/singleton.h"

//------------------------------------------------------------------------------
namespace Http
{
class HttpInterface : public Messaging::AsyncPort
{
    __DeclareClass(HttpInterface);
    __DeclareInterfaceSingleton(HttpInterface);
public:
    /// constructor
    HttpInterface();
    /// destructor
    virtual ~HttpInterface();
    /// open the interface object
    virtual void Open();
    
protected:
    /// derive in subclass, create and attach handlers from here
    virtual void OnCreateHandlers();
};
        
} // namespace HttpInterface
//------------------------------------------------------------------------------
#endif
    
    