//------------------------------------------------------------------------------
//  graphicshandler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/graphicshandler.h"
#include "graphics/graphicsinterface.h"
#include "io/filestream.h"
#include "resources/simpleresourcemapper.h"
#include "coregraphics/streamtextureloader.h"
#include "coregraphics/streammeshloader.h"
#include "coregraphics/shaperenderer.h"
#include "resources/managedtexture.h"
#include "resources/managedmesh.h"
#include "coreanimation/streamanimationloader.h"
#include "internalgraphics/stagebuilder.h"
#include "internalgraphics/internalstage.h"
#include "internalgraphics/internalview.h"
#include "lighting/internalspotlightentity.h"
#include "apprender/platformconfig.h"
#include "debugrender/debugshaperenderer.h"
#include "models/nodes/statenodeinstance.h"
#include "coregraphics/debug/displaypagehandler.h"
#include "coregraphics/debug/texturepagehandler.h"
#include "coregraphics/debug/meshpagehandler.h"
#include "coregraphics/debug/shaderpagehandler.h"

namespace Graphics
{
__ImplementClass(Graphics::GraphicsHandler, 'GHDL', Messaging::Handler);

using namespace IO;
using namespace CoreGraphics;
using namespace CoreAnimation;
using namespace Resources;
using namespace Models;
using namespace InternalGraphics;
using namespace Lighting;
using namespace Frame;
using namespace Anim;
using namespace Messaging;
using namespace Util;
using namespace Debug;

//------------------------------------------------------------------------------
/**
*/
GraphicsHandler::GraphicsHandler() :
    isGraphicsRuntimeValid(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
GraphicsHandler::~GraphicsHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::Open()
{
    n_assert(!this->IsOpen());
    n_assert(!this->isGraphicsRuntimeValid);
    Handler::Open();

    // setup core runtime
    this->ioConsole = IO::Console::Create();
    this->ioConsole->Open();
    this->ioServer = IO::IoServer::Create();
    this->ioServer->RegisterStandardUriSchemes();
    this->ioServer->SetupStandardAssigns();
    this->ioServer->MountStandardZipArchives();

    // setup http page handlers
    this->httpServerProxy = Http::HttpServerProxy::Create();
    this->httpServerProxy->Open();
    this->httpServerProxy->AttachRequestHandler(Debug::DisplayPageHandler::Create());
    this->httpServerProxy->AttachRequestHandler(Debug::MeshPageHandler::Create());
    this->httpServerProxy->AttachRequestHandler(Debug::ShaderPageHandler::Create());
    this->httpServerProxy->AttachRequestHandler(Debug::TexturePageHandler::Create());

    // setup the required objects, but do not open them, this will
    // happen at a later time when the SetupGrapics message is received
    this->renderDevice = RenderDevice::Create();
    this->displayDevice = DisplayDevice::Create();
    this->transformDevice = TransformDevice::Create();
    this->shaderServer = ShaderServer::Create();
    this->shapeRenderer = ShapeRenderer::Create();
    this->textRenderer = TextRenderer::Create();
    this->vertexLayoutServer = VertexLayoutServer::Create();
    this->sharedResourceServer = SharedResourceServer::Create();
    this->resourceManager = ResourceManager::Create();
    this->modelServer = ModelServer::Create();
    this->graphicsServer = InternalGraphicsServer::Create();
    this->lightServer = LightServer::Create();
    this->shadowServer = ShadowServer::Create();
    this->frameServer = FrameServer::Create();
    this->animationServer = AnimationServer::Create();

    // setup timer
    this->timer.Start();

    // setup debug timers and counters
    _setup_timer(GraphicsFrameTime);
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::Close()
{
    n_assert(this->IsOpen());

    _discard_timer(GraphicsFrameTime);
    
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
    this->textRenderer = 0;
    
    this->httpServerProxy->Close();
    this->httpServerProxy = 0;

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
GraphicsHandler::SetupGraphicsRuntime()
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
        n_error("GraphicsHandler: failed to open display device!");
    }
    if (!this->renderDevice->Open())
    {
        n_error("GraphicsHandler: failed to open render device!");
    }
    this->vertexLayoutServer->Open();
    this->shaderServer->Open();
    this->transformDevice->Open();
    this->shapeRenderer->Open();
    this->sharedResourceServer->Open();
    this->resourceManager->Open();
    this->frameServer->Open();
    this->textRenderer->Open();

    // setup resource mapper for textures
    // FIXME: should be configurable!
    Ptr<SimpleResourceMapper> texMapper = SimpleResourceMapper::Create();
    texMapper->SetPlaceholderResourceId(ResourceId(PLACEHOLDER_TEXTURENAME));
    texMapper->SetResourceClass(Texture::RTTI);
    texMapper->SetResourceLoaderClass(StreamTextureLoader::RTTI);
    texMapper->SetManagedResourceClass(ManagedTexture::RTTI);
    this->resourceManager->AttachMapper(texMapper.upcast<ResourceMapper>());

    // setup resource mapper for meshes
    // FIXME: should be configurable!
    Ptr<SimpleResourceMapper> meshMapper = SimpleResourceMapper::Create();
    meshMapper->SetPlaceholderResourceId(ResourceId(PLACEHOLDER_MESHNAME));
    meshMapper->SetResourceClass(Mesh::RTTI);
    meshMapper->SetResourceLoaderClass(StreamMeshLoader::RTTI);
    meshMapper->SetManagedResourceClass(ManagedMesh::RTTI);
    this->resourceManager->AttachMapper(meshMapper.upcast<ResourceMapper>());

    // setup resource mapper for animations
    // FIXME: should be configurable!
    //Ptr<SimpleResourceMapper> animMapper = SimpleResourceMapper::Create();
    //animMapper->SetPlaceholderResourceId(ResourceId("ani:examples/eagle.nax2"));
    //animMapper->SetResourceClass(MemoryAnimation::RTTI);
    //animMapper->SetResourceLoaderClass(StreamAnimationLoader::RTTI);
    //animMapper->SetManagedResourceClass(ManagedAnimation::RTTI);
    //this->resourceManager->AttachMapper(animMapper.upcast<ResourceMapper>());

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
GraphicsHandler::ShutdownGraphicsRuntime()
{
    n_assert(this->isGraphicsRuntimeValid);
    this->isGraphicsRuntimeValid = false;

    this->textRenderer->Close();
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
GraphicsHandler::HandleMessage(const Ptr<Message>& msg)
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
    else if (msg->CheckId(CreateModelEntity::Id))
    {
        this->OnCreateModelEntity(msg.cast<CreateModelEntity>());
    }
    else if (msg->CheckId(CreateGlobalLightEntity::Id))
    {
        this->OnCreateGlobalLightEntity(msg.cast<CreateGlobalLightEntity>());
    }
    else if (msg->CheckId(CreateSpotLightEntity::Id))
    {
        this->OnCreateSpotLightEntity(msg.cast<CreateSpotLightEntity>());
    }
    else if (msg->CheckId(UpdateTransform::Id))
    {
        this->OnUpdateTransform(msg.cast<UpdateTransform>());
    }
    else if (msg->CheckId(AttachCameraToView::Id))
    {
        this->OnAttachCameraToView(msg.cast<AttachCameraToView>());
    }
    else if (msg->CheckId(RemoveCameraFromView::Id))
    {
        this->OnRemoveCameraFromView(msg.cast<RemoveCameraFromView>());
    }
    else if (msg->CheckId(SetVisibility::Id))
    {
        this->OnSetVisibility(msg.cast<SetVisibility>());
    }
    else if (msg->CheckId(Graphics::UpdateInstanceShaderVariable::Id))
    {
        this->pendingGfxMessages.Append(msg);        
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
GraphicsHandler::DoWork()
{
    if (this->isGraphicsRuntimeValid)
    {
        _start_timer(GraphicsFrameTime);
        n_assert(this->IsOpen());

        // first signal the global frameEvent, to notify other threads
        // that we are finished with message processing
        GraphicsInterface::Instance()->SignalFrameEvent();

        // handle pending messages
        this->HandlePendingMessages();

        // finally, render the frame
        this->graphicsServer->OnFrame(this->timer.GetTime());

        _stop_timer(GraphicsFrameTime);
    }

    // allow render-thread HttpRequests to be processed
    this->httpServerProxy->HandlePendingRequests();
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnSetupGraphics(const Ptr<SetupGraphics>& msg)
{
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
GraphicsHandler::OnAdapterExists(const Ptr<AdapterExists>& msg)
{
    msg->SetResult(this->displayDevice->AdapterExists(msg->GetAdapter()));
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnGetAvailableDisplayModes(const Ptr<GetAvailableDisplayModes>& msg)
{
    Adapter::Code adapter = msg->GetAdapter();
    PixelFormat::Code pixelFormat = msg->GetPixelFormat();
    msg->SetResult(this->displayDevice->GetAvailableDisplayModes(adapter, pixelFormat));
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnGetCurrentAdapterDisplayMode(const Ptr<GetCurrentAdapterDisplayMode>& msg)
{
    Adapter::Code adapter = msg->GetAdapter();
    msg->SetResult(this->displayDevice->GetCurrentAdapterDisplayMode(adapter));
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnGetAdapterInfo(const Ptr<GetAdapterInfo>& msg)
{
    Adapter::Code adapter = msg->GetAdapter();
    msg->SetResult(this->displayDevice->GetAdapterInfo(adapter));
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnAttachDisplayEventHandler(const Ptr<AttachDisplayEventHandler>& msg)
{
    this->displayDevice->AttachEventHandler(msg->GetHandler().upcast<DisplayEventHandler>());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnRemoveDisplayEventHandler(const Ptr<RemoveDisplayEventHandler>& msg)
{
    this->displayDevice->RemoveEventHandler(msg->GetHandler().upcast<DisplayEventHandler>());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnAttachRenderEventHandler(const Ptr<AttachRenderEventHandler>& msg)
{
    this->renderDevice->AttachEventHandler(msg->GetHandler().upcast<RenderEventHandler>());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnRemoveRenderEventHandler(const Ptr<RemoveRenderEventHandler>& msg)
{
    this->renderDevice->RemoveEventHandler(msg->GetHandler().upcast<RenderEventHandler>());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnCreateGraphicsStage(const Ptr<CreateGraphicsStage>& msg)
{
    const StringAtom& name = msg->GetName();
    const Core::Rtti* stageBuilderClass = msg->GetStageBuilderClass();
    n_assert(stageBuilderClass && stageBuilderClass->IsDerivedFrom(StageBuilder::RTTI));
    const Attr::AttributeContainer& stageBuilderAttrs = msg->GetStageBuilderAttrs();
    Ptr<StageBuilder> stageBuilder = (StageBuilder*) stageBuilderClass->Create();
    stageBuilder->SetAttributes(stageBuilderAttrs);
    Ptr<InternalStage> stage = InternalGraphicsServer::Instance()->CreateStage(name, stageBuilder);
    msg->SetResult(stage.get());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnDiscardGraphicsStage(const Ptr<DiscardGraphicsStage>& msg)
{
    Ptr<InternalStage> stage = (InternalStage*) msg->GetStageHandle();
    n_assert(stage.isvalid() && stage->IsA(InternalStage::RTTI));
    InternalGraphicsServer::Instance()->DiscardStage(stage);
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnCreateGraphicsView(const Ptr<CreateGraphicsView>& msg)
{
    const Core::Rtti* viewClass = msg->GetViewClass();
    n_assert(viewClass && viewClass->IsDerivedFrom(InternalView::RTTI));
    const StringAtom& viewName = msg->GetName();
    const StringAtom& stageName = msg->GetStageName();
    const ResourceId& frameShaderName = msg->GetFrameShaderName();
    bool isDefaultView = msg->GetDefaultView();

    const Ptr<FrameShader> frameShader = FrameServer::Instance()->GetFrameShaderByName(frameShaderName);
    const Ptr<InternalStage> stage = InternalGraphicsServer::Instance()->GetStageByName(stageName);
    Ptr<InternalView> view = InternalGraphicsServer::Instance()->CreateView(*viewClass, viewName, isDefaultView);
    view->SetStage(stage);
    view->SetFrameShader(frameShader);

    msg->SetResult(view.get());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnDiscardGraphicsView(const Ptr<DiscardGraphicsView>& msg)
{
    Ptr<InternalView> view = (InternalView*) msg->GetViewHandle();
    n_assert(view.isvalid() && view->IsA(InternalView::RTTI));
    InternalGraphicsServer::Instance()->DiscardView(view);
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnCreateCameraEntity(const Ptr<CreateCameraEntity>& msg)
{
    Ptr<InternalCameraEntity> cameraEntity = InternalCameraEntity::Create();
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

    // attach camera entity to stage
    const Ptr<InternalStage>& stage = InternalGraphicsServer::Instance()->GetStageByName(msg->GetStageName());
    stage->AttachEntity(cameraEntity.cast<InternalGraphicsEntity>());

    // set return value
    msg->SetEntityHandle(cameraEntity.get());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnDiscardGraphicsEntity(const Ptr<DiscardGraphicsEntity>& msg)
{
    Ptr<InternalGraphicsEntity> entity = (InternalGraphicsEntity*) msg->GetEntityHandle();
    n_assert(entity.isvalid() && entity->IsA(InternalGraphicsEntity::RTTI));
    Ptr<InternalStage> stage = entity->GetStage();
    n_assert(stage.isvalid());
    stage->RemoveEntity(entity);
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnCreateModelEntity(const Ptr<CreateModelEntity>& msg)
{
    // create a new model entity
    Ptr<InternalModelEntity> modelEntity = InternalModelEntity::Create();
    modelEntity->SetTransform(msg->GetTransform());
    modelEntity->SetVisible(msg->GetVisible());
    modelEntity->SetResourceId(msg->GetResourceId());

    // lookup stage and attach entity
    const Ptr<InternalStage>& stage = InternalGraphicsServer::Instance()->GetStageByName(msg->GetStageName());
    stage->AttachEntity(modelEntity.cast<InternalGraphicsEntity>());

    // set return value
    msg->SetEntityHandle(modelEntity.get());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnCreateGlobalLightEntity(const Ptr<CreateGlobalLightEntity>& msg)
{
    // create a new global light entity
    Ptr<Lighting::InternalGlobalLightEntity> lightEntity = Lighting::InternalGlobalLightEntity::Create();
    lightEntity->SetTransform(msg->GetTransform());
    lightEntity->SetVisible(msg->GetVisible());
    lightEntity->SetColor(msg->GetLightColor());
    lightEntity->SetBackLightColor(msg->GetBackLightColor());
    lightEntity->SetCastShadows(msg->GetCastShadows());
    lightEntity->SetProjMapUvOffsetAndScale(msg->GetProjMapUvOffsetAndScale());
    
    // lookup stage and attach entity
    const Ptr<InternalStage>& stage = InternalGraphicsServer::Instance()->GetStageByName(msg->GetStageName());
    stage->AttachEntity(lightEntity.cast<InternalGraphicsEntity>());

    // set return value
    msg->SetEntityHandle(lightEntity.get());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnCreateSpotLightEntity(const Ptr<CreateSpotLightEntity>& msg)
{
    // create a new spotlight entity
    Ptr<Lighting::InternalSpotLightEntity> lightEntity = Lighting::InternalSpotLightEntity::Create();
    lightEntity->SetTransform(msg->GetTransform());
    lightEntity->SetVisible(msg->GetVisible());
    lightEntity->SetColor(msg->GetLightColor());
    lightEntity->SetCastShadows(msg->GetCastShadows());
    lightEntity->SetProjMapUvOffsetAndScale(msg->GetProjMapUvOffsetAndScale());

    // lookup stage and attach entity
    const Ptr<InternalStage>& stage = InternalGraphicsServer::Instance()->GetStageByName(msg->GetStageName());
    stage->AttachEntity(lightEntity.cast<InternalGraphicsEntity>());

    // set return value
    msg->SetEntityHandle(lightEntity.get());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnUpdateTransform(const Ptr<UpdateTransform>& msg)
{
    Ptr<InternalGraphicsEntity> entity = (InternalGraphicsEntity*) msg->GetEntityHandle();
    n_assert(entity.isvalid() && entity->IsA(InternalGraphicsEntity::RTTI));
    entity->SetTransform(msg->GetTransform());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnSetVisibility(const Ptr<SetVisibility>& msg)
{
    Ptr<InternalGraphicsEntity> entity = (InternalGraphicsEntity*) msg->GetEntityHandle();
    n_assert(entity.isvalid() && entity->IsA(InternalGraphicsEntity::RTTI));
    entity->SetVisible(msg->GetVisible());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnAttachCameraToView(const Ptr<AttachCameraToView>& msg)
{
    Ptr<InternalCameraEntity> entity = (InternalCameraEntity*) msg->GetEntityHandle();
    n_assert(entity.isvalid() && entity->IsA(InternalCameraEntity::RTTI));
    const StringAtom& viewName = msg->GetViewName();
    n_assert(InternalGraphicsServer::Instance()->HasView(viewName));
    InternalGraphicsServer::Instance()->GetViewByName(viewName)->SetCameraEntity(entity);
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnRemoveCameraFromView(const Ptr<RemoveCameraFromView>& msg)
{
    Ptr<InternalCameraEntity> entity = (InternalCameraEntity*) msg->GetEntityHandle();
    n_assert(entity.isvalid() && entity->IsA(InternalCameraEntity::RTTI));
    const StringAtom& viewName = msg->GetViewName();
    n_assert(InternalGraphicsServer::Instance()->HasView(viewName));
    const Ptr<InternalView>& view = InternalGraphicsServer::Instance()->GetViewByName(viewName);
    n_assert(view->GetCameraEntity() == entity);
    view->SetCameraEntity(0);
}

//------------------------------------------------------------------------------
/**
*/
bool
GraphicsHandler::OnUpdateInstanceShaderVariable(const Ptr<Graphics::UpdateInstanceShaderVariable>& msg)
{
    Ptr<InternalGraphicsEntity> entity = (InternalGraphicsEntity*) msg->GetEntityHandle();
    n_assert(entity.isvalid() && entity->IsA(InternalGraphicsEntity::RTTI));
    n_assert(entity->GetType() == InternalGraphics::InternalGraphicsEntityType::Model);
    Ptr<InternalModelEntity> modelEntity = entity.cast<InternalModelEntity>();
    // is modelentity deleted, and msg out-of-date, return handled = true to remove msg from list
    if (!modelEntity->IsActive())
    {
        return true;
    }

    if (modelEntity->GetModelResourceState() == Resources::Resource::Loaded)
    {
        const Ptr<ModelInstance>& modelInst =  modelEntity->GetModelInstance();    
        if (modelInst.isvalid())
        {
            const Util::StringAtom& name = msg->GetNodeInstanceName();
            const Ptr<ModelNodeInstance>& nodeInst = modelInst->LookupNodeInstance(name);
            n_assert(nodeInst->IsA(StateNodeInstance::RTTI));
            const Ptr<StateNodeInstance>& stateNodeInst = nodeInst.cast<StateNodeInstance>();
            Ptr<CoreGraphics::ShaderVariableInstance> var;
            if (stateNodeInst->HasShaderVariableInstance(ShaderVariable::Semantic(msg->GetSemantic())))
            {
                var = stateNodeInst->GetShaderVariableInstance(ShaderVariable::Semantic(msg->GetSemantic()));        
            }
            else
            {
                var = stateNodeInst->CreateShaderVariableInstance(ShaderVariable::Semantic(msg->GetSemantic()));
            }

            const Util::Variant& value = msg->GetValue();
            var->SetValue(value);   

            return true;
        }
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::HandlePendingMessages()
{
    IndexT i;
    for (i = 0; i < this->pendingGfxMessages.Size(); i++)
    {
        if (this->pendingGfxMessages[i]->CheckId(Graphics::UpdateInstanceShaderVariable::Id))
        {
            if (this->OnUpdateInstanceShaderVariable(this->pendingGfxMessages[i].cast<Graphics::UpdateInstanceShaderVariable>()))
            {
                this->pendingGfxMessages.EraseIndex(i);
                i--;
            }
        }
    }
}

} // namespace Graphics

