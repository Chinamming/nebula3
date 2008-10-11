#ifndef GRAPHICS_GRAPHICSINTERFACE_H
#define GRAPHICS_GRAPHICSINTERFACE_H
//------------------------------------------------------------------------------
/**
    @class Graphics::GraphicsInterface
  
    Implements the asynchronous interface to the InternalGraphics subsystem. 
    Usually the application doesn't call methods on the GraphicsInterface
    directly, but instead uses one of the Graphics objects to
    communicate with the graphics thread.
    
    (C) 2007 Radon Labs GmbH
*/    
#include "messaging/asyncport.h"
#include "core/singleton.h"

//------------------------------------------------------------------------------
namespace Graphics
{
class GraphicsInterface : public Messaging::AsyncPort
{
    __DeclareClass(GraphicsInterface);
    __DeclareInterfaceSingleton(GraphicsInterface);
public:
    /// constructor
    GraphicsInterface();
    /// destructor
    virtual ~GraphicsInterface();
    /// open the interface object
    virtual void Open();
    /// wait for the graphics thread to process messages
    void WaitForFrameEvent();

protected:
    friend class GraphicsHandler;

    /// derive in subclass, create and attach handlers from here
    virtual void OnCreateHandlers();
    /// signal the per-frame event, called by GraphicsHandler
    void SignalFrameEvent();

    Threading::Event frameEvent;
};

} // namespace Graphics
//------------------------------------------------------------------------------
#endif
