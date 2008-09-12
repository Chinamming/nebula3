//------------------------------------------------------------------------------
//  graphicsinterface.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/graphicsinterface.h"
#include "graphics/graphicshandler.h"

namespace Graphics
{
ImplementClass(Graphics::GraphicsInterface, 'GRIF', Messaging::AsyncPort);
ImplementInterfaceSingleton(Graphics::GraphicsInterface);

//------------------------------------------------------------------------------
/**
*/
GraphicsInterface::GraphicsInterface()
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
GraphicsInterface::~GraphicsInterface()
{
    DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsInterface::Open()
{
    // we need to run continously, not wait for messages
    this->SetWaitForMessages(false);
    this->SetName("GraphicsInterface");
    AsyncPort::Open();
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsInterface::OnCreateHandlers()
{
    Ptr<GraphicsHandler> msgHandler = GraphicsHandler::Create();
    this->AttachHandler(msgHandler.upcast<Messaging::Handler>());
}

//------------------------------------------------------------------------------
/**
    This method is called by the AsyncGraphicsHandler once per frame
    right after pending messages are processed and before starting
    frame rendering. This can be used to synchronize other threads
    with rendering, for instance to prevent the game thread from running
    ahead of the graphics thread and thus spamming its message queue.
*/
void
GraphicsInterface::SignalFrameEvent()
{
    this->frameEvent.Signal();
}

//------------------------------------------------------------------------------
/**
    This method can be called by any thread who wishes to synchronize itself
    with the graphics thread. The event will become signalled once per
    graphics frame after messages are processed and before rendering starts.
    The best place for a thread to call this method is right before graphics
    messages for the next frame are produced.
*/
void
GraphicsInterface::WaitForFrameEvent()
{
    this->frameEvent.Wait();
}

} // namespace Graphics
