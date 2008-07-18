#pragma once
#ifndef ASYNCGRAPHICS_ASYNCGRAPHICSHANDLER_H
#define ASYNCGRAPHICS_ASYNCGRAPHICSHANDLER_H
//------------------------------------------------------------------------------
/**
    @class AsyncGraphics::AsyncGraphicsHandler
    
    Runs in the graphics thread context, setsup the graphics runtime environment
    and processes messages to the graphics thread.

    (C) 2008 Radon Labs GmbH
*/
#include "messaging/handler.h"
#include "messaging/message.h"
#include "asyncgraphics/protocol.h"
#include "io/console.h"
#include "io/ioserver.h"
#include "coregraphics/renderdevice.h"
#include "coregraphics/displaydevice.h"
#include "coregraphics/transformdevice.h"
#include "coregraphics/shaderserver.h"
#include "coregraphics/shaperenderer.h"
#include "coregraphics/vertexlayoutserver.h"
#include "resources/sharedresourceserver.h"
#include "resources/resourcemanager.h"
#include "models/modelserver.h"
#include "graphics/graphicsserver.h"
#include "lighting/lightserver.h"
#include "lighting/shadowserver.h"
#include "frame/frameserver.h"
#include "addons/nebula2/nebula2wrapper.h"
#include "timing/timer.h"

//------------------------------------------------------------------------------
namespace AsyncGraphics
{
class AsyncGraphicsHandler : public Messaging::Handler
{
    DeclareClass(AsyncGraphicsHandler);
public:
    /// constructor
    AsyncGraphicsHandler();
    /// destructor
    virtual ~AsyncGraphicsHandler();

    /// open the handler
    virtual void Open();
    /// close the handler
    virtual void Close();
    /// handle a message, return true if handled
    virtual bool HandleMessage(const Ptr<Messaging::Message>& msg);
    /// do per-frame work
    virtual void DoWork();

private:
    /// setup the required graphics runtime
    void SetupGraphicsRuntime();
    /// shutdown the graphics runtime
    void ShutdownGraphicsRuntime();
    /// handle SetupGraphics message
    void OnSetupGraphics(const Ptr<AsyncGraphics::SetupGraphics>& msg);
    /// handle AdapterExists message
    void OnAdapterExists(const Ptr<AsyncGraphics::AdapterExists>& msg);
    /// handle GetAvailableDisplayModes message
    void OnGetAvailableDisplayModes(const Ptr<AsyncGraphics::GetAvailableDisplayModes>& msg);
    /// handle GetCurrentAdapterDisplayMode message
    void OnGetCurrentAdapterDisplayMode(const Ptr<AsyncGraphics::GetCurrentAdapterDisplayMode>& msg);
    /// handle GetAdapterInfo message
    void OnGetAdapterInfo(const Ptr<AsyncGraphics::GetAdapterInfo>& msg);
    /// handle AttachDisplayEventHandler message
    void OnAttachDisplayEventHandler(const Ptr<AsyncGraphics::AttachDisplayEventHandler>& msg);
    /// handle RemoveDisplayEventHandler message
    void OnRemoveDisplayEventHandler(const Ptr<AsyncGraphics::RemoveDisplayEventHandler>& msg);
    /// handle AttachRenderEventHandler message
    void OnAttachRenderEventHandler(const Ptr<AsyncGraphics::AttachRenderEventHandler>& msg);
    /// handle RemoveRenderEventHandler message
    void OnRemoveRenderEventHandler(const Ptr<AsyncGraphics::RemoveRenderEventHandler>& msg);
    /// handle CreateGraphicsStage message
    void OnCreateGraphicsStage(const Ptr<AsyncGraphics::CreateGraphicsStage>& msg);
    /// handle a DiscardGraphicsStage message
    void OnDiscardGraphicsStage(const Ptr<AsyncGraphics::DiscardGraphicsStage>& msg);
    /// handle CreateGraphicsView message
    void OnCreateGraphicsView(const Ptr<AsyncGraphics::CreateGraphicsView>& msg);
    /// handle DiscardGraphicsView message
    void OnDiscardGraphicsView(const Ptr<AsyncGraphics::DiscardGraphicsView>& msg);
    /// handle CreateCameraEntity message
    void OnCreateCameraEntity(const Ptr<AsyncGraphics::CreateCameraEntity>& msg);
    /// handle DiscardGraphicsEntity message
    void OnDiscardGraphicsEntity(const Ptr<AsyncGraphics::DiscardGraphicsEntity>& msg);

    bool isGraphicsRuntimeValid;
    Timing::Timer timer;
    Ptr<IO::Console> ioConsole;
    Ptr<IO::IoServer> ioServer;
    Ptr<CoreGraphics::RenderDevice> renderDevice;
    Ptr<CoreGraphics::DisplayDevice> displayDevice;
    Ptr<CoreGraphics::TransformDevice> transformDevice;
    Ptr<CoreGraphics::ShaderServer> shaderServer;
    Ptr<CoreGraphics::ShapeRenderer> shapeRenderer;
    Ptr<CoreGraphics::VertexLayoutServer> vertexLayoutServer;
    Ptr<Resources::SharedResourceServer> sharedResourceServer;
    Ptr<Resources::ResourceManager> resourceManager;
    Ptr<Models::ModelServer> modelServer;
    Ptr<Graphics::GraphicsServer> graphicsServer;
    Ptr<Lighting::LightServer> lightServer;
    Ptr<Lighting::ShadowServer> shadowServer;
    Ptr<Frame::FrameServer> frameServer;
    Ptr<Anim::AnimationServer> animationServer;
};

} // namespace AsyncGraphicsHandler
//------------------------------------------------------------------------------
#endif
