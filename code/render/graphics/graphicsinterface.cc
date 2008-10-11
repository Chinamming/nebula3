//------------------------------------------------------------------------------
//  graphicsinterface.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/graphicsinterface.h"
#include "graphics/graphicshandler.h"
#include "debugrender/debuggraphicshandler.h"
#include "ui/uigraphicshandler.h"

namespace Graphics
{
__ImplementClass(Graphics::GraphicsInterface, 'GRIF', Messaging::AsyncPort);
__ImplementInterfaceSingleton(Graphics::GraphicsInterface);

//------------------------------------------------------------------------------
/**
*/
GraphicsInterface::GraphicsInterface()
{
    __ConstructSingleton;
    this->SetThreadCpuCoreId(System::Cpu::RenderThreadCore);
}

//------------------------------------------------------------------------------
/**
*/
GraphicsInterface::~GraphicsInterface()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsInterface::Open()
{
    // we need to run continously, not wait for messages
    this->SetBehaviour(DoNotWait);
    this->SetName("GraphicsInterface Thread");
    AsyncPort::Open();
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsInterface::OnCreateHandlers()
{
    Ptr<GraphicsHandler> graphicsHandler = GraphicsHandler::Create();
    this->AttachHandler(graphicsHandler.upcast<Messaging::Handler>());
    
	Ptr<Debug::DebugGraphicsHandler> debugGraphicsHandler = Debug::DebugGraphicsHandler::Create();
    this->AttachHandler(debugGraphicsHandler.upcast<Messaging::Handler>());

	Ptr<CoreUI::UIGraphicsHandler> uiGraphicsHandler = CoreUI::UIGraphicsHandler::Create();
	this->AttachHandler(uiGraphicsHandler.upcast<Messaging::Handler>());
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
