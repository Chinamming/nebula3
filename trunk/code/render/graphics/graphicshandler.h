#pragma once
#ifndef GRAPHICS_GRAPHICSHANDLER_H
#define GRAPHICS_GRAPHICSHANDLER_H
//------------------------------------------------------------------------------
/**
    @class Graphics::GraphicsHandler
    
    Runs in the graphics thread context, setsup the graphics runtime environment
    and processes messages to the graphics thread.

    (C) 2008 Radon Labs GmbH
*/
#include "messaging/handler.h"
#include "messaging/message.h"
#include "graphics/graphicsprotocol.h"
#include "debugrender/debugrenderprotocol.h"
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
#include "internalgraphics/internalgraphicsserver.h"
#include "lighting/lightserver.h"
#include "lighting/shadowserver.h"
#include "frame/frameserver.h"
#include "addons/nebula2/nebula2wrapper.h"
#include "timing/timer.h"

#include "debug/debugtimer.h"
#include "coregraphics/textrenderer.h"
#include "http/httpserverproxy.h"

//------------------------------------------------------------------------------
namespace Graphics
{
class GraphicsHandler : public Messaging::Handler
{
    __DeclareClass(GraphicsHandler);
public:
    /// constructor
    GraphicsHandler();
    /// destructor
    virtual ~GraphicsHandler();

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
    void OnSetupGraphics(const Ptr<Graphics::SetupGraphics>& msg);
    /// handle AdapterExists message
    void OnAdapterExists(const Ptr<Graphics::AdapterExists>& msg);
    /// handle GetAvailableDisplayModes message
    void OnGetAvailableDisplayModes(const Ptr<Graphics::GetAvailableDisplayModes>& msg);
    /// handle GetCurrentAdapterDisplayMode message
    void OnGetCurrentAdapterDisplayMode(const Ptr<Graphics::GetCurrentAdapterDisplayMode>& msg);
    /// handle GetAdapterInfo message
    void OnGetAdapterInfo(const Ptr<Graphics::GetAdapterInfo>& msg);
    /// handle AttachDisplayEventHandler message
    void OnAttachDisplayEventHandler(const Ptr<Graphics::AttachDisplayEventHandler>& msg);
    /// handle RemoveDisplayEventHandler message
    void OnRemoveDisplayEventHandler(const Ptr<Graphics::RemoveDisplayEventHandler>& msg);
    /// handle AttachRenderEventHandler message
    void OnAttachRenderEventHandler(const Ptr<Graphics::AttachRenderEventHandler>& msg);
    /// handle RemoveRenderEventHandler message
    void OnRemoveRenderEventHandler(const Ptr<Graphics::RemoveRenderEventHandler>& msg);
    /// handle CreateGraphicsStage message
    void OnCreateGraphicsStage(const Ptr<Graphics::CreateGraphicsStage>& msg);
    /// handle a DiscardGraphicsStage message
    void OnDiscardGraphicsStage(const Ptr<Graphics::DiscardGraphicsStage>& msg);
    /// handle CreateGraphicsView message
    void OnCreateGraphicsView(const Ptr<Graphics::CreateGraphicsView>& msg);
    /// handle DiscardGraphicsView message
    void OnDiscardGraphicsView(const Ptr<Graphics::DiscardGraphicsView>& msg);
    /// handle CreateCameraEntity message
    void OnCreateCameraEntity(const Ptr<Graphics::CreateCameraEntity>& msg);
    /// handle CreateModelEntity message
    void OnCreateModelEntity(const Ptr<Graphics::CreateModelEntity>& msg);
    /// handle CreateGlobalLightEntity message
    void OnCreateGlobalLightEntity(const Ptr<Graphics::CreateGlobalLightEntity>& msg);
    /// handle CreateSpotLightEntity message
    void OnCreateSpotLightEntity(const Ptr<Graphics::CreateSpotLightEntity>& msg);
    /// handle DiscardGraphicsEntity message
    void OnDiscardGraphicsEntity(const Ptr<Graphics::DiscardGraphicsEntity>& msg);
    /// handle UpdateTransform message
    void OnUpdateTransform(const Ptr<Graphics::UpdateTransform>& msg);
    /// handle AttachCameraToView message
    void OnAttachCameraToView(const Ptr<Graphics::AttachCameraToView>& msg);
    /// handle RemoveCameraFromView message
    void OnRemoveCameraFromView(const Ptr<Graphics::RemoveCameraFromView>& msg);
    /// handle SetVisibility message
    void OnSetVisibility(const Ptr<Graphics::SetVisibility>& msg);
    /// on update instance variable
    bool OnUpdateInstanceShaderVariable(const Ptr<Graphics::UpdateInstanceShaderVariable>& msg);
    /// handle pending messages
    void HandlePendingMessages();

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
    Ptr<CoreGraphics::TextRenderer> textRenderer;
    Ptr<Resources::SharedResourceServer> sharedResourceServer;
    Ptr<Resources::ResourceManager> resourceManager;
    Ptr<Models::ModelServer> modelServer;
    Ptr<InternalGraphics::InternalGraphicsServer> graphicsServer;
    Ptr<Lighting::LightServer> lightServer;
    Ptr<Lighting::ShadowServer> shadowServer;
    Ptr<Frame::FrameServer> frameServer;
    Ptr<Anim::AnimationServer> animationServer;

    Util::Array<Ptr<Messaging::Message> > pendingGfxMessages;
    
    _declare_timer(GraphicsFrameTime);

    Ptr<Http::HttpServerProxy> httpServerProxy;
};

} // namespace GraphicsHandler
//------------------------------------------------------------------------------
#endif
