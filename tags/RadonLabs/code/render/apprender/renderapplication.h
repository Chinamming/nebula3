#pragma once
#ifndef APP_RENDERAPPLICATION_H
#define APP_RENDERAPPLICATION_H
//------------------------------------------------------------------------------
/**
    @class App::RenderApplication
    
    Base class for Nebula3 applications with 3d rendering capability.
    
    (C) 2007 Radon Labs GmbH
*/
#include "app/application.h"
#include "core/coreserver.h"
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
#include "interface/iointerface.h"
#include "input/inputserver.h"
#include "frame/frameserver.h"
#if !__WII__
#include "scripting/scriptserver.h"
#include "http/httpserver.h"
#endif
#include "timing/timer.h"
#include "addons/nebula2/nebula2wrapper.h"
#include "apprender/platformconfig.h"

//------------------------------------------------------------------------------
namespace App
{
class RenderApplication : public Application
{
public:
    /// constructor
    RenderApplication();
    /// destructor
    virtual ~RenderApplication();
    /// open the application
    virtual bool Open();
    /// close the application
    virtual void Close();
    /// run the application
    virtual void Run();

private:
    /// update the current time values
    void UpdateTime();

protected:
    /// set quit requested flag
    void SetQuitRequested(bool b);
    /// return true if quit requested
    bool IsQuitRequested() const;
    /// called to configure display device
    virtual void OnConfigureDisplayDevice();
    /// process input (called before rendering)
    virtual void OnProcessInput();
    /// update world 
    virtual void OnUpdateFrame();
    /// render current frame
    virtual void OnRenderFrame();
    /// get current absolute time
    Timing::Time GetTime() const;
    /// get current frame time
    Timing::Time GetFrameTime() const;

    Ptr<Core::CoreServer> coreServer;
    Ptr<IO::IoServer> ioServer;
    Ptr<Interface::IOInterface> ioInterface;   
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
    Ptr<Input::InputServer> inputServer;
    Ptr<Frame::FrameServer> frameServer;
#if !__WII__    
    Ptr<Scripting::ScriptServer> scriptServer;
    Ptr<Http::HttpServer> httpServer;
#endif    
    Ptr<Anim::AnimationServer> animationServer;
    Timing::Timer timer;
    Timing::Time time;
    Timing::Time frameTime;
    bool quitRequested;
};

//------------------------------------------------------------------------------
/**
*/
inline void
RenderApplication::SetQuitRequested(bool b)
{
    this->quitRequested = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
RenderApplication::IsQuitRequested() const
{
    return this->quitRequested;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Time
RenderApplication::GetTime() const
{
    return this->time;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Time
RenderApplication::GetFrameTime() const
{
    return this->frameTime;
}

} // namespace App
//------------------------------------------------------------------------------
#endif
