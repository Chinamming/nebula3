#ifndef ASYNCGRAPHICS_ASYNCGRAPHICSINTERFACE_H
#define ASYNCGRAPHICS_ASYNCGRAPHICSINTERFACE_H
//------------------------------------------------------------------------------
/**
    @class AsyncGraphics::AsyncGraphicsInterface
  
    Implements the asynchronous interface to the Graphics subsystem. Usually
    the application doesn't call methods on the AsyncGraphicsInterface
    directly, but instead uses one of the AsyncGraphics proxy objects to
    communicate with the graphics thread.
    
    (C) 2007 Radon Labs GmbH
*/    
#include "messaging/asyncport.h"
#include "core/singleton.h"

//------------------------------------------------------------------------------
namespace AsyncGraphics
{
class AsyncGraphicsInterface : public Messaging::AsyncPort
{
    DeclareClass(AsyncGraphicsInterface);
    DeclareInterfaceSingleton(AsyncGraphicsInterface);
public:
    /// constructor
    AsyncGraphicsInterface();
    /// destructor
    virtual ~AsyncGraphicsInterface();
    /// open the interface object
    virtual void Open();

protected:
    /// derive in subclass, create and attach handlers from here
    virtual void OnCreateHandlers();
};

} // namespace AsyncGraphics
//------------------------------------------------------------------------------
#endif
