//------------------------------------------------------------------------------
//  game/graphicsfeature.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphicsfeature/graphicsfeatureunit.h"
#include "graphicsfeature/graphicsfeatureproperties.h"
#include "graphics/stagebuilders.h"
#include "internalgraphics/internalview.h"  // FIXME! should have to use InternalGraphics!
#include "apprender/platformconfig.h"
#include "game/gameserver.h"
#include "debugrender/debugrenderprotocol.h"
#include "threading/thread.h"

namespace GraphicsFeature
{
__ImplementClass(GraphicsFeatureUnit, 'FGFX' , Game::FeatureUnit);
__ImplementSingleton(GraphicsFeatureUnit);

using namespace CoreGraphics;
using namespace Graphics;
using namespace Input;
using namespace Resources;
using namespace Threading;

//------------------------------------------------------------------------------
/**
*/
GraphicsFeatureUnit::GraphicsFeatureUnit()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
GraphicsFeatureUnit::~GraphicsFeatureUnit()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::OnActivate()
{
    n_assert(!this->IsActive());
    n_assert(!this->defaultStage.isvalid());
    n_assert(!this->defaultView.isvalid());
    
    // call parent class
    FeatureUnit::OnActivate();

    // setup the graphics subsystem
    this->graphicsInterface = GraphicsInterface::Create();
    this->graphicsInterface->Open();
    this->display = Display::Create();
    this->OnConfigureDisplay();
    this->display->Open();
    this->graphicsServer = GraphicsServer::Create();
    this->graphicsServer->Open();
	this->debugShapeRenderer = Debug::DebugShapeRenderer::Create();
    this->debugTextRenderer = Debug::DebugTextRenderer::Create();

    // setup input subsystem
    this->inputServer = InputServer::Create();
    this->inputServer->Open();
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::OnDeactivate()
{
    n_assert(this->IsActive());
    n_assert(!this->defaultStage.isvalid());
    n_assert(!this->defaultView.isvalid());
    n_assert(!this->defaultCamera.isvalid());

    this->inputServer->Close();
    this->inputServer = 0;
    this->graphicsServer->Close();
    this->graphicsServer = 0;
    this->display->Close();
    this->display = 0;
    this->graphicsInterface->Close();
    this->graphicsInterface = 0;
	this->debugShapeRenderer = 0;
    this->debugTextRenderer = 0;

    FeatureUnit::OnDeactivate();
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::OnStart()
{
    // reset input events, e.g. needed after new gamestate is set
    this->inputServer->Reset();
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::SetupDefaultGraphicsWorld()
{
    n_assert(this->IsActive());
    n_assert(!this->defaultStage.isvalid());
    n_assert(!this->defaultView.isvalid());
    n_assert(!this->defaultCamera.isvalid());

    ResourceId defaultStageName("DefaultStage");
    ResourceId defaultViewName("DefaultView");
    
    // setup a default stage and view
    Attr::AttributeContainer stageBuilderAttrs;
    this->defaultStage = this->graphicsServer->CreateStage(defaultStageName, 
                                                           Graphics::SimpleStageBuilder::RTTI, 
                                                           stageBuilderAttrs);
    this->defaultView = this->graphicsServer->CreateView(InternalGraphics::InternalView::RTTI,
                                                         defaultViewName,
                                                         defaultStageName,
                                                         ResourceId(DEFAULT_FRAMESHADER_NAME),
                                                         true);

    // setup a default camera
    this->defaultCamera = CameraEntity::Create();
    this->defaultStage->AttachEntity(this->defaultCamera.cast<GraphicsEntity>());
    this->defaultView->SetCameraEntity(this->defaultCamera);
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::DiscardDefaultGraphicsWorld()
{
    n_assert(this->defaultStage.isvalid());
    n_assert(this->defaultView.isvalid());
    n_assert(this->defaultCamera.isvalid());

    this->defaultStage->RemoveEntity(this->defaultCamera.cast<GraphicsEntity>());
    this->defaultCamera = 0;
    this->graphicsServer->DiscardAllStages();
    this->graphicsServer->DiscardAllViews();
    this->defaultView = 0;
    this->defaultStage = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::OnBeginFrame()
{  
    this->inputServer->BeginFrame();
    this->inputServer->OnFrame();
    GraphicsInterface::Instance()->WaitForFrameEvent();
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::OnFrame()
{    
	this->debugShapeRenderer->OnFrame();
    this->debugTextRenderer->OnFrame();
    GraphicsServer::Instance()->OnFrame();
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::OnEndFrame()
{
    if (this->inputServer->IsQuitRequested())
    {
        Game::GameServer::Instance()->SetQuitRequested();
    }
    this->inputServer->EndFrame();

    // in windowed mode, give other apps time slice
    if (!this->display->IsFullscreen())
    {
        Core::SysFunc::Sleep(0.0);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::StartRenderDebug()
{   
    Ptr<Debug::RenderDebugView> renderDebugMsg = Debug::RenderDebugView::Create();
    renderDebugMsg->SetThreadId(Thread::GetMyThreadId());
    renderDebugMsg->SetEnableDebugRendering(true);
    Graphics::GraphicsInterface::Instance()->Send(renderDebugMsg.cast<Messaging::Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::StopRenderDebug()
{   
    Ptr<Debug::RenderDebugView> renderDebugMsg = Debug::RenderDebugView::Create();
    renderDebugMsg->SetThreadId(Thread::GetMyThreadId());
    renderDebugMsg->SetEnableDebugRendering(false);    
    Graphics::GraphicsInterface::Instance()->Send(renderDebugMsg.cast<Messaging::Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::OnConfigureDisplay()
{
    // display adapter
    Adapter::Code adapter = Adapter::Primary;
    if (this->args.HasArg("-adapter"))
    {
        adapter = Adapter::FromString(this->args.GetString("-adapter"));
        if (this->display->AdapterExists(adapter))
        {
            this->display->SetAdapter(adapter);
        }
    }

    // display mode
    DisplayMode displayMode;
    if (this->args.HasArg("-x"))
    {
        displayMode.SetXPos(this->args.GetInt("-x"));
    }
    if (this->args.HasArg("-y"))
    {
        displayMode.SetYPos(this->args.GetInt("-y"));
    }
    if (this->args.HasArg("-w"))
    {
        displayMode.SetWidth(this->args.GetInt("-w"));
    }
    if (this->args.HasArg("-h"))
    {
        displayMode.SetHeight(this->args.GetInt("-h"));
    }
    this->display->SetDisplayMode(displayMode);
    this->display->SetFullscreen(this->args.GetBool("-fullscreen"));
    this->display->SetAlwaysOnTop(this->args.GetBool("-alwaysontop"));
    this->display->SetVerticalSyncEnabled(this->args.GetBool("-vsync"));
    if (this->args.HasArg("-aa"))
    {
        this->display->SetAntiAliasQuality(AntiAliasQuality::FromString(this->args.GetString("-aa")));
    }
}

} // namespace GraphicsFeature
