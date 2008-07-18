//------------------------------------------------------------------------------
//  asyncgraphicshandler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "asyncgraphics/asyncgraphicshandler.h"
#include "io/filestream.h"
#include "resources/simpleresourcemapper.h"
#include "coregraphics/streamtextureloader.h"
#include "coregraphics/streammeshloader.h"
#include "resources/managedtexture.h"
#include "resources/managedmesh.h"
#include "coregraphics/streamanimationloader.h"
#include "graphics/stagebuilder.h"
#include "graphics/stage.h"
#include "graphics/view.h"

namespace AsyncGraphics
{
ImplementClass(AsyncGraphics::AsyncGraphicsHandler, 'AGHR', Messaging::Handler);

using namespace IO;
using namespace CoreGraphics;
using namespace Resources;
using namespace Models;
using namespace Graphics;
using namespace Lighting;
using namespace Frame;
using namespace Anim;
using namespace Messaging;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
AsyncGraphicsHandler::AsyncGraphicsHandler() :
    isGraphicsRuntimeValid(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
AsyncGraphicsHandler::~AsyncGraphicsHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
AsyncGraphicsHandler::Open()
{
    n_assert(!this->IsOpen());
    n_assert(!this->isGraphicsRuntimeValid);
    Handler::Open();

    // setup core runtime
    this->ioConsole = IO::Console::Create();
    this->ioConsole->Open();
    this->ioServer = IO::IoServer::Create();

    // register URI schemes and defines 
    // FIXME: redundant! IoInterface should be sole IO-Server-Replacement!
    this->ioServer->RegisterUriScheme("file", FileStream::RTTI);
    this->ioServer->SetAssign(Assign("shd", "home:export/shaders"));
    this->ioServer->SetAssign(Assign("frame", "home:export/frame"));
    this->ioServer->SetAssign(Assign("msh", "home:export/meshes"));
    this->ioServer->SetAssign(Assign("tex", "home:export/textures"));
    this->ioServer->SetAssign(Assign("ani", "home:export/anims"));
    this->ioServer->SetAssign(Assign("mdl", "home:export/gfxlib"));        

    // Nebula2 backward compat assigns:
    // FIXME: redundant! IoInterface should be sole IO-Server-Replacement!
    this->ioServer->SetAssign(Assign("meshes", "home:export/meshes"));
    this->ioServer->SetAssign(Assign("textures", "home:export/textures"));
    this->ioServer->SetAssign(Assign("anims", "home:export/anims"));

    // mount asset zip archives
    // FIXME: redundant! IoInterface should be sole IO-Server-Replacement!
    if (IoServer::Instance()->FileExists("home:export.zip"))
    {
        this->ioServer->MountZipArchive("home:export.zip");
    }

    // setup the required objects, but do not open them, this will
    // happen at a later time when the SetupGrapics message is received
    this->renderDevice = RenderDevice::Create();
    this->displayDevice = DisplayDevice::Create();
    this->transformDevice = TransformDevice::Create();
    this->shaderServer = ShaderServer::Create();
    this->shapeRenderer = ShapeRenderer::Create();
    this->vertexLayoutServer = VertexLayoutServer::Create();
    this->sharedResourceServer = SharedResourceServer::Create();
    this->resourceManager = ResourceManager::Create();
    this->modelServer = ModelServer::Create();
    this->graphicsServer = GraphicsServer::Create();
    this->lightServer = LightServer::Create();
    this->shadowServer = ShadowServer::Create();
    this->frameServer = FrameServer::Create();
    this->animationServer = AnimationServer::Create();

    // setup timer
    this->timer.Start();
}

//------------------------------------------------------------------------------
/**
*/
void
AsyncGraphicsHandler::Close()
{
    n_assert(this->IsOpen());
    if (this->isGraphicsRuntimeValid)
    {
        this->ShutdownGraphicsRuntime();
    }
    this->frameServer = 0;
    this->shadowServer = 0;
    this->lightServer = 0;
    this->graphicsServer = 0;
    this->modelServer = 0;
    this->resourceManager = 0;
    this->sharedResourceServer = 0;
    this->vertexLayoutServer = 0;
    this->shapeRenderer = 0;
    this->shaderServer = 0;
    this->transformDevice = 0;
    this->displayDevice = 0;
    this->renderDevice = 0;
    this->animationServer = 0;
    this->ioServer = 0;
    this->ioConsole = 0;
    Handler::Close();
}

//------------------------------------------------------------------------------
/**
    Setup the graphics runtime, this method is called when the
    SetupGraphics message is received from the main thread. The method
    expects that the DisplayDevice has been configured with the 
    desired display settings.
*/
void
AsyncGraphicsHandler::SetupGraphicsRuntime()
{
    n_assert(!this->isGraphicsRuntimeValid);
    n_assert(!this->displayDevice->IsOpen());
    n_assert(!this->renderDevice->IsOpen());
    n_assert(!this->transformDevice->IsOpen());
    n_assert(!this->shaderServer->IsOpen());
    n_assert(!this->shapeRenderer->IsOpen());
    n_assert(!this->vertexLayoutServer->IsOpen());
    n_assert(!this->sharedResourceServer->IsOpen());
    n_assert(!this->resourceManager->IsOpen());
    n_assert(!this->modelServer->IsOpen());
    n_assert(!this->graphicsServer->IsOpen());
    n_assert(!this->lightServer->IsOpen());
    n_assert(!this->shadowServer->IsOpen());
    n_assert(!this->frameServer->IsOpen());

    if (!this->displayDevice->Open())
    {
        n_error("AsyncGraphicsHandler: failed to open display device!");
    }
    if (!this->renderDevice->Open())
    {
        n_error("AsyncGraphicsHandler: failed to open render device!");
    }
    this->vertexLayoutServer->Open();
    this->shaderServer->Open();
    this->transformDevice->Open();
    this->shapeRenderer->Open();
    this->sharedResourceServer->Open();
    this->resourceManager->Open();
    this->frameServer->Open();

    // setup resource mapper for textures
    // FIXME: should be configurable!
    Ptr<SimpleResourceMapper> texMapper = SimpleResourceMapper::Create();
    texMapper->SetPlaceholderResourceId(ResourceId("tex:system/placeholder.dds"));
    texMapper->SetResourceClass(Texture::RTTI);
    texMapper->SetResourceLoaderClass(StreamTextureLoader::RTTI);
    texMapper->SetManagedResourceClass(ManagedTexture::RTTI);
    this->resourceManager->AttachMapper(texMapper.upcast<ResourceMapper>());

    // setup resource mapper for meshes
    // FIXME: should be configurable!
    Ptr<SimpleResourceMapper> meshMapper = SimpleResourceMapper::Create();
    meshMapper->SetPlaceholderResourceId(ResourceId("msh:system/placeholder_s_0.nvx2"));
    meshMapper->SetResourceClass(Mesh::RTTI);
    meshMapper->SetResourceLoaderClass(StreamMeshLoader::RTTI);
    meshMapper->SetManagedResourceClass(ManagedMesh::RTTI);
    this->resourceManager->AttachMapper(meshMapper.upcast<ResourceMapper>());

    // setup resource mapper for animations
    // FIXME: should be configurable!
    Ptr<SimpleResourceMapper> animMapper = SimpleResourceMapper::Create();
    animMapper->SetPlaceholderResourceId(ResourceId("ani:examples/eagle.nax2"));
    animMapper->SetResourceClass(MemoryAnimation::RTTI);
    animMapper->SetResourceLoaderClass(StreamAnimationLoader::RTTI);
    animMapper->SetManagedResourceClass(ManagedAnimation::RTTI);
    this->resourceManager->AttachMapper(animMapper.upcast<ResourceMapper>());

    this->modelServer->Open();
    this->graphicsServer->Open();
    this->lightServer->Open();
    this->shadowServer->Open();

    this->isGraphicsRuntimeValid = true;
}

//------------------------------------------------------------------------------
/**
    Shutdown the graphics runtime, this is called from the Close() method.
*/
void
AsyncGraphicsHandler::ShutdownGraphicsRuntime()
{
    n_assert(this->isGraphicsRuntimeValid);
    this->isGraphicsRuntimeValid = 0;

    this->shadowServer->Close();
    this->lightServer->Close();
    this->graphicsServer->Close();
    this->modelServer->Close();
    this->resourceManager->Close();
    this->frameServer->Close();
    this->sharedResourceServer->Close();
    this->shapeRenderer->Close();
    this->shaderServer->Close();
    this->vertexLayoutServer->Close();
    this->renderDevice->Close();
    this->displayDevice->Close();
}

//------------------------------------------------------------------------------
/**
*/
bool
AsyncGraphicsHandler::HandleMessage(const Ptr<Message>& msg)
{
    n_assert(msg.isvalid());
    if (msg->CheckId(SetupGraphics::Id))
    {
        this->OnSetupGraphics(msg.cast<SetupGraphics>());
    }
    else if (msg->CheckId(AdapterExists::Id))
    {
        this->OnAdapterExists(msg.cast<AdapterExists>());
    }
    else if (msg->CheckId(GetAvailableDisplayModes::Id))
    {
        this->OnGetAvailableDisplayModes(msg.cast<GetAvailableDisplayModes>());
    }
    else if (msg->CheckId(GetCurrentAdapterDisplayMode::Id))
    {
        this->OnGetCurrentAdapterDisplayMode(msg.cast<GetCurrentAdapterDisplayMode>());
    }
    else if (msg->CheckId(GetAdapterInfo::Id))
    {
        this->OnGetAdapterInfo(msg.cast<GetAdapterInfo>());
    }
    else if (msg->CheckId(AttachDisplayEventHandler::Id))
    {
        this->OnAttachDisplayEventHandler(msg.cast<AttachDisplayEventHandler>());
    }
    else if (msg->CheckId(RemoveDisplayEventHandler::Id))
    {
        this->OnRemoveDisplayEventHandler(msg.cast<RemoveDisplayEventHandler>());
    }
    else if (msg->CheckId(AttachRenderEventHandler::Id))
    {
        this->OnAttachRenderEventHandler(msg.cast<AttachRenderEventHandler>());
    }
    else if (msg->CheckId(RemoveRenderEventHandler::Id))
    {
        this->OnRemoveRenderEventHandler(msg.cast<RemoveRenderEventHandler>());
    }
    else if (msg->CheckId(CreateGraphicsStage::Id))
    {
        this->OnCreateGraphicsStage(msg.cast<CreateGraphicsStage>());
    }
    else if (msg->CheckId(DiscardGraphicsStage::Id))
    {
        this->OnDiscardGraphicsStage(msg.cast<DiscardGraphicsStage>());
    }
    else if (msg->CheckId(CreateGraphicsView::Id))
    {
        this->OnCreateGraphicsView(msg.cast<CreateGraphicsView>());
    }
    else if (msg->CheckId(DiscardGraphicsView::Id))
    {
        this->OnDiscardGraphicsView(msg.cast<DiscardGraphicsView>());
    }
    else if (msg->CheckId(CreateCameraEntity::Id))
    {
        this->OnCreateCameraEntity(msg.cast<CreateCameraEntity>());
    }
    else if (msg->CheckId(DiscardGraphicsEntity::Id))
    {
        this->OnDiscardGraphicsEntity(msg.cast<DiscardGraphicsEntity>());
    }
    else
    {
        // unknown message
        return false;
    }
    // fallthrough: message was handled
    return true;
}

//------------------------------------------------------------------------------
/**
    This is the per-frame method which implements the asynchronous render-loop.
*/
void
AsyncGraphicsHandler::DoWork()
{
    n_assert(this->IsOpen());
    this->graphicsServer->OnFrame(this->timer.GetTime());
}

//------------------------------------------------------------------------------
/**
*/
void
AsyncGraphicsHandler::OnSetupGraphics(const Ptr<SetupGraphics>& msg)
{
    n_printf("AsyncGraphicsHandler::OnSetupGraphics() called!\n");
    n_assert(!this->isGraphicsRuntimeValid);

    // configure the display device and setup the graphics runtime
    DisplayDevice* disp = this->displayDevice;
    disp->SetAdapter(msg->GetAdapter());
    disp->SetDisplayMode(msg->GetDisplayMode());
    disp->SetAntiAliasQuality(msg->GetAntiAliasQuality());
    disp->SetFullscreen(msg->GetFullscreen());
    disp->SetDisplayModeSwitchEnabled(msg->GetDisplayModeSwitchEnabled());
    disp->SetTripleBufferingEnabled(msg->GetTripleBufferingEnabled());
    disp->SetAlwaysOnTop(msg->GetAlwaysOnTop());
    disp->SetVerticalSyncEnabled(msg->GetVerticalSyncEnabled());
    disp->SetIconName(msg->GetIconName());
    this->SetupGraphicsRuntime();
}

//------------------------------------------------------------------------------
/**
*/
void
AsyncGraphicsHandler::OnAdapterExists(const Ptr<AdapterExists>& msg)
{
    n_printf("AsyncGraphicsHandler::OnAdapterExists() called!\n");
    msg->SetResult(this->displayDevice->AdapterExists(msg->GetAdapter()));
}

//------------------------------------------------------------------------------
/**
*/
void
AsyncGraphicsHandler::OnGetAvailableDisplayModes(const Ptr<GetAvailableDisplayModes>& msg)
{
    n_printf("AsyncGraphicsHandler::OnGetAvailableDisplayModes() called!\n");
    Adapter::Code adapter = msg->GetAdapter();
    PixelFormat::Code pixelFormat = msg->GetPixelFormat();
    msg->SetResult(this->displayDevice->GetAvailableDisplayModes(adapter, pixelFormat));
}

//------------------------------------------------------------------------------
/**
*/
void
AsyncGraphicsHandler::OnGetCurrentAdapterDisplayMode(const Ptr<GetCurrentAdapterDisplayMode>& msg)
{
    n_printf("AsyncGraphicsHandler::OnGetCurrentAdapterDisplayMode() called!\n");
    Adapter::Code adapter = msg->GetAdapter();
    msg->SetResult(this->displayDevice->GetCurrentAdapterDisplayMode(adapter));
}

//------------------------------------------------------------------------------
/**
*/
void
AsyncGraphicsHandler::OnGetAdapterInfo(const Ptr<GetAdapterInfo>& msg)
{
    n_printf("AsyncGraphicsHandler::OnGetAdapterInfo() called!\n");
    Adapter::Code adapter = msg->GetAdapter();
    msg->SetResult(this->displayDevice->GetAdapterInfo(adapter));
}

//------------------------------------------------------------------------------
/**
*/
void
AsyncGraphicsHandler::OnAttachDisplayEventHandler(const Ptr<AttachDisplayEventHandler>& msg)
{
    n_printf("AsyncGraphicsHandler::OnAttachDisplayEventHandler() called!\n");
    this->displayDevice->AttachEventHandler(msg->GetHandler().upcast<DisplayEventHandler>());
}

//------------------------------------------------------------------------------
/**
*/
void
AsyncGraphicsHandler::OnRemoveDisplayEventHandler(const Ptr<RemoveDisplayEventHandler>& msg)
{
    n_printf("AsyncGraphicsHandler::OnRemoveDisplayEventHandler() called!\n");
    this->displayDevice->RemoveEventHandler(msg->GetHandler().upcast<DisplayEventHandler>());
}

//------------------------------------------------------------------------------
/**
*/
void
AsyncGraphicsHandler::OnAttachRenderEventHandler(const Ptr<AttachRenderEventHandler>& msg)
{
    n_printf("AsyncGraphicsHandler::OnAttachRenderEventHandler() called!\n");
    this->renderDevice->AttachEventHandler(msg->GetHandler().upcast<RenderEventHandler>());
}

//------------------------------------------------------------------------------
/**
*/
void
AsyncGraphicsHandler::OnRemoveRenderEventHandler(const Ptr<RemoveRenderEventHandler>& msg)
{
    n_printf("AsyncGraphicsHandler::OnRemoveRenderEventHandler() called!\n");
    this->renderDevice->RemoveEventHandler(msg->GetHandler().upcast<RenderEventHandler>());
}

//------------------------------------------------------------------------------
/**
*/
void
AsyncGraphicsHandler::OnCreateGraphicsStage(const Ptr<CreateGraphicsStage>& msg)
{
    n_printf("AsyncGraphicsHandler::OnCreateGraphicsStage() called!\n");
    const StringAtom& name = msg->GetName();
    const Core::Rtti* stageBuilderClass = msg->GetStageBuilderClass();
    n_assert(stageBuilderClass && stageBuilderClass->IsDerivedFrom(StageBuilder::RTTI));
    const Attr::AttributeContainer& stageBuilderAttrs = msg->GetStageBuilderAttrs();
    Ptr<StageBuilder> stageBuilder = (StageBuilder*) stageBuilderClass->Create();
    stageBuilder->SetAttributes(stageBuilderAttrs);
    Ptr<Stage> stage = GraphicsServer::Instance()->CreateStage(name, stageBuilder);
    msg->SetResult(stage.get());
}

//------------------------------------------------------------------------------
/**
*/
void
AsyncGraphicsHandler::OnDiscardGraphicsStage(const Ptr<DiscardGraphicsStage>& msg)
{
    n_printf("AsyncGraphicsHandler::OnDiscardGraphicsStage() called!\n");
    Ptr<Stage> stage = (Stage*) msg->GetStageHandle();
    n_assert(stage.isvalid() && stage->IsA(Stage::RTTI));
    GraphicsServer::Instance()->DiscardStage(stage);
}

//------------------------------------------------------------------------------
/**
*/
void
AsyncGraphicsHandler::OnCreateGraphicsView(const Ptr<CreateGraphicsView>& msg)
{
    n_printf("AsyncGraphicsHandler::OnCreateGraphicsView() called!\n");
    
    const Core::Rtti* viewClass = msg->GetViewClass();
    n_assert(viewClass && viewClass->IsDerivedFrom(View::RTTI));
    const StringAtom& viewName = msg->GetName();
    const StringAtom& stageName = msg->GetStageName();
    const ResourceId& frameShaderName = msg->GetFrameShaderName();
    bool isDefaultView = msg->GetDefaultView();

    const Ptr<FrameShader> frameShader = FrameServer::Instance()->GetFrameShaderByName(frameShaderName);
    const Ptr<Stage> stage = GraphicsServer::Instance()->GetStageByName(stageName);
    Ptr<View> view = GraphicsServer::Instance()->CreateView(*viewClass, viewName, isDefaultView);
    view->SetStage(stage);
    view->SetFrameShader(frameShader);

    msg->SetResult(view.get());
}

//------------------------------------------------------------------------------
/**
*/
void
AsyncGraphicsHandler::OnDiscardGraphicsView(const Ptr<DiscardGraphicsView>& msg)
{
    n_printf("AsyncGraphicsHandler::OnDiscardGraphicsView() called!\n");

    Ptr<View> view = (View*) msg->GetViewHandle();
    n_assert(view.isvalid() && view->IsA(View::RTTI));
    GraphicsServer::Instance()->DiscardView(view);
}

//------------------------------------------------------------------------------
/**
*/
void
AsyncGraphicsHandler::OnCreateCameraEntity(const Ptr<CreateCameraEntity>& msg)
{
    n_printf("AsyncGraphicsHandler::OnCreateCameraEntity() called!\n");

    Ptr<CameraEntity> cameraEntity = CameraEntity::Create();
    cameraEntity->SetTransform(msg->GetTransform());
    cameraEntity->SetVisible(msg->GetVisible());
    if (msg->GetIsPerspective())
    {
        cameraEntity->SetupPerspectiveFov(msg->GetPerspFieldOfView(), 
                                          msg->GetPerspAspectRatio(), 
                                          msg->GetNearPlane(), 
                                          msg->GetFarPlane());
    }
    else
    {
        cameraEntity->SetupOrthogonal(msg->GetOrthoWidth(),
                                      msg->GetOrthoHeight(),
                                      msg->GetNearPlane(),
                                      msg->GetFarPlane());
    }

    // attach camera entity to stage and view
    const Ptr<Stage>& stage = GraphicsServer::Instance()->GetStageByName(msg->GetStageName());
    const Ptr<View>& view = GraphicsServer::Instance()->GetViewByName(msg->GetViewName());
    stage->AttachEntity(cameraEntity.cast<GraphicsEntity>());
    view->SetCameraEntity(cameraEntity);

    // set return value
    msg->SetEntityHandle(cameraEntity.get());
}

//------------------------------------------------------------------------------
/**
*/
void
AsyncGraphicsHandler::OnDiscardGraphicsEntity(const Ptr<DiscardGraphicsEntity>& msg)
{
    n_printf("AsyncGraphicsHandler::OnDiscardGraphicsEntity() called!\n");

    Ptr<GraphicsEntity> entity = (GraphicsEntity*) msg->GetEntityHandle();
    n_assert(entity.isvalid() && entity->IsA(GraphicsEntity::RTTI));
    Ptr<Stage> stage = entity->GetStage();
    n_assert(stage.isvalid());
    stage->RemoveEntity(entity);
}

} // namespace AsyncGraphics
