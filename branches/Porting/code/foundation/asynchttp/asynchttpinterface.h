#pragma once
#ifndef ASYNCHTTP_ASYNCHTTPINTERFACE_H
#define ASYNCHTTP_ASYNCHTTPINTERFACE_H
//------------------------------------------------------------------------------
/**
    @class AsyncHttp::AsyncHttpInterface
    
    Central interface object of the asynchronous Http subsystem.
    
    (C) 2008 Radon Labs GmbH
*/
#include "messaging/asyncport.h"
#include "core/singleton.h"

//------------------------------------------------------------------------------
namespace AsyncHttp
{
class AsyncHttpInterface : public Messaging::AsyncPort
{
    DeclareClass(AsyncHttpInterface);
    DeclareInterfaceSingleton(AsyncHttpInterface);
public:
    /// constructor
    AsyncHttpInterface();
    /// destructor
    virtual ~AsyncHttpInterface();
    /// open the interface object
    virtual void Open();
    
protected:
    /// derive in subclass, create and attach handlers from here
    virtual void OnCreateHandlers();
};
        
} // namespace AsyncHttpInterface
//------------------------------------------------------------------------------
#endif
    
    