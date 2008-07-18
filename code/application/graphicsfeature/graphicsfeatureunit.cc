//------------------------------------------------------------------------------
//  game/graphicsfeature.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphicsfeature/graphicsfeatureunit.h"
#include "corefeature/corefeatureunit.h"
#include "game/gameserver.h"
#include "app/application.h"
#include "io/filestream.h"
#include "resources/simpleresourcemapper.h"
#include "coregraphics/texture.h"
#include "coregraphics/mesh.h"
#include "coregraphics/streamtextureloader.h"
#include "coregraphics/streammeshloader.h"
#include "coregraphics/streamanimationloader.h"
#include "resources/managedtexture.h"
#include "resources/managedmesh.h"
#include "graphics/quadtreestagebuilder.h"
#include "graphics/simplestagebuilder.h"
#include "math/bbox.h"
#include "lighting/spotlightentity.h"
#include "input/keyboard.h"
#include "managers/focusmanager.h"
#include "basegametiming/systemtimesource.h"
#include "addons/nebula2/nebula2wrapper.h"
#include "apprender/platformconfig.h"

#if __USE_HTTP__
#include "http/httpserver.h"
#include "coregraphics/debug/displaypagehandler.h"
#include "coregraphics/debug/texturepagehandler.h"
#include "coregraphics/debug/meshpagehandler.h"
#include "coregraphics/debug/shaderpagehandler.h"
#endif

// include all properties for known by managers::factorymanager
#include "properties/inputproperty.h"
#include "properties/graphicsproperty.h"
#include "properties/actorgraphicsproperty.h"
#include "properties/environmentgraphicsproperty.h"
#include "properties/lightproperty.h"
#include "properties/cameraproperty.h"
#include "properties/chasecameraproperty.h"
#include "properties/simplemoveproperty.h"

namespace GraphicsFeature
{
ImplementClass(GraphicsFeatureUnit, 'FGFX' , Game::FeatureUnit);
ImplementSingleton(GraphicsFeatureUnit);

using namespace App;
using namespace Core;
using namespace Util;
using namespace Math;
using namespace IO;
using namespace Graphics;
using namespace CoreGraphics;
using namespace Resources;
using namespace Frame;
using namespace Models;
using namespace Lighting;
using namespace Input;
using namespace Game;
using namespace CoreFeature;
using namespace BaseGameFeature;
using namespace Nebula2;

//------------------------------------------------------------------------------
/**
*/
GraphicsFeatureUnit::GraphicsFeatureUnit():
    renderActiveDebugModes(false),
    activeDebugModeIndex(0)
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
GraphicsFeatureUnit::~GraphicsFeatureUnit()
{
    DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::OnActivate()
{
	FeatureUnit::OnActivate();

    // this feature needs the core feature
    n_assert2(CoreFeatureUnit::HasInstance(), "GraphicsFeatureUnit needs the CoreFeature!");
    n_assert(CoreFeatureUnit::Instance()->IsActive());

#if __USE_HTTP__
    Http::HttpServer::Instance()->AttachRequestHandler(Debug::DisplayPageHandler::Create());
    Http::HttpServer::Instance()->AttachRequestHandler(Debug::TexturePageHandler::Create());
    Http::HttpServer::Instance()->AttachRequestHandler(Debug::MeshPageHandler::Create());
    Http::HttpServer::Instance()->AttachRequestHandler(Debug::ShaderPageHandler::Create());
#endif
    
    // setup core graphics subsystem
    this->displayDevice = DisplayDevice::Create();
    this->OnConfigureDisplayDevice();
    if (!this->displayDevice->Open())
    {
        n_error("RenderApplication: Failed to open display device!");        
    }
    this->renderDevice = RenderDevice::Create();
    if (!this->renderDevice->Open())
    {
        n_error("RenderApplication: Failed to open render device!");
    }
    this->vertexLayoutServer = VertexLayoutServer::Create();
    this->vertexLayoutServer->Open();
    this->shaderServer = ShaderServer::Create();
    this->shaderServer->Open();
    this->transformDevice = TransformDevice::Create();
    this->transformDevice->Open();
    this->shapeRenderer = ShapeRenderer::Create();
    this->shapeRenderer->Open();

    // setup resource subsystem
    this->sharedResourceServer = SharedResourceServer::Create();
    this->sharedResourceServer->Open();
    this->resourceManager = ResourceManager::Create();
    this->resourceManager->Open();

    // setup frame server
    this->frameServer = FrameServer::Create();
    this->frameServer->Open();

    // setup resource mapper for textures
    Ptr<SimpleResourceMapper> texMapper = SimpleResourceMapper::Create();
    texMapper->SetPlaceholderResourceId(ResourceId(PLACEHOLDER_TEXTURENAME));
    texMapper->SetResourceClass(Texture::RTTI);
    texMapper->SetResourceLoaderClass(StreamTextureLoader::RTTI);
    texMapper->SetManagedResourceClass(ManagedTexture::RTTI);
    this->resourceManager->AttachMapper(texMapper.upcast<ResourceMapper>());

    // setup resource mapper for meshes
    Ptr<SimpleResourceMapper> meshMapper = SimpleResourceMapper::Create();
    meshMapper->SetPlaceholderResourceId(ResourceId("msh:system/placeholder_s_0.nvx2"));
    meshMapper->SetResourceClass(Mesh::RTTI);
    meshMapper->SetResourceLoaderClass(StreamMeshLoader::RTTI);
    meshMapper->SetManagedResourceClass(ManagedMesh::RTTI);
    this->resourceManager->AttachMapper(meshMapper.upcast<ResourceMapper>());

    // setup resource mapper for animations
    Ptr<SimpleResourceMapper> animMapper = SimpleResourceMapper::Create();
    animMapper->SetPlaceholderResourceId(ResourceId("ani:examples/eagle.nax2"));
    animMapper->SetResourceClass(Nebula2::nMemoryAnimation::RTTI);
    animMapper->SetResourceLoaderClass(StreamAnimationLoader::RTTI);
    animMapper->SetManagedResourceClass(ManagedAnimation::RTTI);
    this->resourceManager->AttachMapper(animMapper.upcast<ResourceMapper>());

    // setup model server
    this->modelServer = ModelServer::Create();
    this->modelServer->Open();

    // setup graphics subsystem
    this->graphicsServer = GraphicsServer::Create();
    this->graphicsServer->Open();

    // setup lighting subsystem
    this->lightServer = LightServer::Create();
    this->lightServer->Open();
    this->shadowServer = ShadowServer::Create();
    this->shadowServer->Open();

    // setup input subsystem
    this->inputServer = InputServer::Create();
    this->inputServer->Open();

    // create animation server
    this->animationServer = Nebula2::nAnimationServer::Create();

    // create character server    
    this->characterServer = Nebula2::CharacterServer::Create();

    // create anim table
    this->animTable = Nebula2::AnimTable::Create();
    
    /// add this feature for render debug callback
    this->AddRenderDebugCallback(this, "Graphics");
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::OnDeactivate()
{
    this->debugCallbacks.Clear();

    this->activeStages.Clear();
    this->activeViews.Clear();
    this->defaultCamera = 0;

    this->inputServer->Close();
    this->inputServer = 0;

    this->shadowServer->Close();
    this->shadowServer = 0;

    this->lightServer->Close();
    this->lightServer = 0;

    this->graphicsServer->Close();
    this->graphicsServer = 0;

    this->modelServer->Close();
    this->modelServer = 0;

    this->resourceManager->Close();
    this->resourceManager = 0;

    this->frameServer->Close();
    this->frameServer = 0;

    this->sharedResourceServer->Close();
    this->sharedResourceServer = 0;

    this->shapeRenderer->Close();
    this->shapeRenderer = 0;

    this->shaderServer->Close();
    this->shaderServer = 0;

    this->transformDevice->Close();
    this->transformDevice = 0;

    this->vertexLayoutServer->Close();
    this->vertexLayoutServer = 0;

    this->renderDevice->Close();
    this->renderDevice = 0;

    this->displayDevice->Close();
    this->displayDevice = 0;

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
GraphicsFeatureUnit::OnBeginFrame()
{  
    this->inputServer->BeginFrame();
    this->inputServer->OnFrame();
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::OnFrame()
{    
    this->OnRenderFrame();

    // FIXME: render debug, need to split up
    // GraphicsServer::OnFrame() into BeginFrame(), RenderFrame(), EndFrame()
    // for this to work!
    /*
    if (this->renderActiveDebugModes)
    {
        this->RenderDebug();
    }
    */
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::OnEndFrame()
{
    if (this->inputServer->IsQuitRequested())
    {
        Game::GameServer::Instance()->RequestQuit();
    }
    this->HandleDebugInputKeys();
    this->inputServer->EndFrame();
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::OnRenderFrame()
{
    Timing::Time curTime = BaseGameFeature::SystemTimeSource::Instance()->GetTime();
    GraphicsServer::Instance()->OnFrame(curTime);
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::OnRenderDebug()
{
    int numViews = this->activeViews.Size();
    int i;
    for (i = 0; i < numViews; i++)
    {
        this->activeViews[i]->RenderDebugSimple();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::OnConfigureDisplayDevice()
{
// display adapter
    Adapter::Code adapter = Adapter::Primary;
    if (this->args.HasArg("-adapter"))
    {
        adapter = Adapter::FromString(this->args.GetString("-adapter"));
        if (this->displayDevice->AdapterExists(adapter))
        {
            this->displayDevice->SetAdapter(adapter);
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
    this->displayDevice->SetDisplayMode(displayMode);
    this->displayDevice->SetFullscreen(this->args.GetBool("-fullscreen"));
    this->displayDevice->SetAlwaysOnTop(this->args.GetBool("-alwaysontop"));
    this->displayDevice->SetVerticalSyncEnabled(this->args.GetBool("-vsync"));
    if (this->args.HasArg("-aa"))
    {
        this->displayDevice->SetAntiAliasQuality(AntiAliasQuality::FromString(this->args.GetString("-aa")));
    }

    // window title
    if (this->args.HasArg("-windowtitle"))
    {
        this->displayDevice->SetWindowTitle(this->args.GetString("-windowtitle"));
    }
    else
    {
        this->displayDevice->SetWindowTitle(App::Application::Instance()->GetAppName());
    }
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::AttachView(const Ptr<Graphics::View>& view)
{
    n_assert(this->activeViews.FindIndex(view) == InvalidIndex);
    this->activeViews.Append(view);
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::RemoveView(const Ptr<Graphics::View>& view)
{    
    IndexT index = this->activeViews.FindIndex(view);
    n_assert(index != InvalidIndex);
    this->activeViews.EraseIndex(index);
}
    
//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::AttachStage(const Ptr<Graphics::Stage>& stage)
{
    n_assert(this->activeStages.FindIndex(stage) == InvalidIndex);
    this->activeStages.Append(stage);
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::RemoveStage(const Ptr<Graphics::Stage>& stage)
{
    IndexT index = this->activeStages.FindIndex(stage);
    n_assert(index != InvalidIndex);
    this->activeStages.EraseIndex(index);
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::CreateDefaultGraphicsWorld()
{
    // add a default stage and view to our graphics stage    
    // setup a stage
    //Ptr<QuadtreeStageBuilder> quadTreeStageBuilder = QuadtreeStageBuilder::Create();    
    //bbox levelBox;
    //levelBox.extend(point(5000.0f, 5000.0f, 5000.0f));    
    //quadTreeStageBuilder->SetQuadTreeSettings(5, levelBox);
    //Ptr<Stage> stage = this->graphicsServer->CreateStage(StringAtom("DefaultStage"), quadTreeStageBuilder.upcast<StageBuilder>());
    Ptr<Stage> stage = this->graphicsServer->CreateStage(StringAtom("DefaultStage"), SimpleStageBuilder::Create()); 
        
    // attach a camera to the stage
    this->defaultCamera = CameraEntity::Create();
    stage->AttachEntity(this->defaultCamera.upcast<GraphicsEntity>());

    // setup a default view
    Ptr<View> view = this->graphicsServer->CreateView(View::RTTI, StringAtom("DefaultView"), true);
    view->SetStage(stage);
    view->SetFrameShader(FrameServer::Instance()->GetFrameShaderByName(ResourceId(DEFAULT_FRAMESHADER_NAME)));
    view->SetCameraEntity(this->defaultCamera);

    // add to our render lists
    this->AttachStage(stage);
    this->AttachView(view);
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::CleanupDefaultGraphicsWorld()
{
    this->defaultLight = 0;
    this->defaultCamera = 0;

    while (this->activeStages.Size() > 0)
    {
        RemoveStage(this->activeStages[0]);
    }

    while (this->activeViews.Size() > 0)
    {
        RemoveView(this->activeViews[0]);
    }
    Graphics::GraphicsServer::Instance()->DiscardAllStages();
    Graphics::GraphicsServer::Instance()->DiscardAllViews();

}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::CreateDefaultLight()
{
    // attach a light entity
    matrix44 lightTransform = matrix44::multiply(matrix44::scaling(100.0f, 100.0f, 100.0f), matrix44::lookatrh(point(20.0f, 20.0f, 20.0f), point::origin(), vector::upvec()));
    this->defaultLight = SpotLightEntity::Create();
    this->defaultLight->SetCastShadows(true);
    this->defaultLight->SetTransform(lightTransform);
    this->defaultLight->SetColor(float4(1.0f, 1.0f, 1.0f, 1.0f));      
    Stage* defaultStage = this->GetDefaultStage();
    defaultStage->AttachEntity(this->defaultLight.upcast<GraphicsEntity>());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::HandleDebugInputKeys()
{
    Input::InputServer* inputServer = Input::InputServer::Instance();
    if (inputServer->GetDefaultKeyboard()->KeyDown(Input::Key::F3))
    {
        // toggle debug rendering 
        ToggleRenderDebug();
    }
    if (inputServer->GetDefaultKeyboard()->KeyDown(Input::Key::F4))
    {
        // switch to next render mode
        SetNextDebugMode();
    }
    if (inputServer->GetDefaultKeyboard()->KeyDown(Input::Key::Tab))
    {
        FocusManager::Instance()->SetFocusToNextEntity();
    }
}

//------------------------------------------------------------------------------
/**
    Renders the debug visualization of the the game features.
*/
void
GraphicsFeatureUnit::RenderDebug()
{
    if (CoreGraphics::ShapeRenderer::HasInstance())
    {
        // setup global transforms...
        CoreGraphics::TransformDevice* transformDevice = CoreGraphics::TransformDevice::Instance();
        Graphics::CameraEntity* cam = GraphicsFeature::GraphicsFeatureUnit::Instance()->GetDefaultView()->GetCameraEntity();
        transformDevice->SetProjTransform(cam->GetProjTransform());
        transformDevice->SetViewTransform(cam->GetViewTransform());

        CoreGraphics::ShapeRenderer* shapeRenderer = CoreGraphics::ShapeRenderer::Instance();
        shapeRenderer->Begin();
        uint i;
        Array<String> activeDebugModeNames;
        for(i = 0; i < this->debugCallbacks.Size(); i++)
        {            
            if (this->debugCallbacks.ValueAtIndex(i).active)
            {
                this->debugCallbacks.ValueAtIndex(i).feature->OnRenderDebug();
                activeDebugModeNames.Append(this->debugCallbacks.KeyAtIndex(i));
            }
        }
        shapeRenderer->End();

        /// TODO: print all active debug mode names on screen
        for (i = 0; i < activeDebugModeNames.Size(); i++)
        {
            
        }
    }
}

}; // namespace Game
