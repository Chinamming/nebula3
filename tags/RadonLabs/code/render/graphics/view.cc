//------------------------------------------------------------------------------
//  view.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/view.h"
#include "graphics/stagebuilder.h"
#include "graphics/modelentity.h"
#include "models/visresolver.h"
#include "coregraphics/transformdevice.h"
#include "coregraphics/shaperenderer.h"
#include "lighting/lightserver.h"
#include "lighting/shadowserver.h"

namespace Graphics
{
ImplementClass(Graphics::View, 'GRVW', Core::RefCounted);

using namespace Util;
using namespace Models;
using namespace CoreGraphics;
using namespace Lighting;

//------------------------------------------------------------------------------
/**
*/
View::View() :
    isAttachedToServer(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
View::~View()
{
    // check that we've been properly cleaned up
    n_assert(!this->isAttachedToServer);
    n_assert(!this->stage.isvalid());
    n_assert(!this->camera.isvalid());
    n_assert(!this->renderTarget.isvalid());
    n_assert(!this->frameShader.isvalid());
    n_assert(this->dependencies.IsEmpty());
}

//------------------------------------------------------------------------------
/**
*/
void
View::OnAttachToServer()
{
    n_assert(!this->IsAttachedToServer());
    this->isAttachedToServer = true;
}

//------------------------------------------------------------------------------
/**
*/
void
View::OnRemoveFromServer()
{
    n_assert(this->IsAttachedToServer());
    if (this->camera.isvalid())
    {
        this->camera->OnRemoveFromView(this);
        this->camera = 0;
    }
    this->stage = 0;
    this->renderTarget = 0;
    this->frameShader = 0;
    this->dependencies.Clear();
    this->isAttachedToServer = false;
}

//------------------------------------------------------------------------------
/**
    This method updates the visibility links for the view's camera.
    This method should usually be called before Render().
*/
void
View::UpdateVisibilityLinks()
{
    n_assert(this->camera.isvalid());
    this->stage->UpdateCameraLinks(this->camera);
}

//------------------------------------------------------------------------------
/**
    This attaches visible lights to the light and shadow server.
    @todo: currently this methods needs to go over all visible
    graphics entities to find the lights... 
*/
void
View::ResolveVisibleLights()
{
    n_assert(this->camera.isvalid());
    LightServer* lightServer = LightServer::Instance();
    ShadowServer* shadowServer = ShadowServer::Instance();

    lightServer->BeginAttachVisibleLights();
    shadowServer->BeginAttachVisibleLights();
    const Array<Ptr<GraphicsEntity> >& visLinks = this->camera->GetLinks(GraphicsEntity::CameraLink);
    IndexT i;
    SizeT num = visLinks.Size();
    for (i = 0; i < num; i++)
    {
        const Ptr<GraphicsEntity>& curEntity = visLinks[i];
        if (GraphicsEntity::LightType == curEntity->GetType())
        {
            const Ptr<AbstractLightEntity>& lightEntity = curEntity.downcast<AbstractLightEntity>();
            lightServer->AttachVisibleLight(lightEntity);
            if (lightEntity->GetCastShadows())
            {
                shadowServer->AttachVisibleLight(lightEntity);
            }
        }
    }
    shadowServer->EndAttachVisibleLights();
    lightServer->EndAttachVisibleLights();
}

//------------------------------------------------------------------------------
/**
    Resolve all visible ModelNodeInstances by following the visibility
    links of our camera. This is necessary as preparation for rendering.
*/
void
View::ResolveVisibleModelNodeInstances()
{
    n_assert(this->camera.isvalid());
    VisResolver* visResolver = VisResolver::Instance();
    visResolver->BeginResolve();
    const Array<Ptr<GraphicsEntity> >& visLinks = this->camera->GetLinks(GraphicsEntity::CameraLink);
    IndexT i;
    SizeT num = visLinks.Size();
    for (i = 0; i < num; i++)
    {
        const Ptr<GraphicsEntity>& curEntity = visLinks[i];
        if (GraphicsEntity::ModelType == curEntity->GetType())
        {
            const Ptr<ModelEntity>& modelEntity = curEntity.downcast<ModelEntity>();
            visResolver->AttachVisibleModelInstance(modelEntity->GetModelInstance());
        }
    }
    visResolver->EndResolve();
}

//------------------------------------------------------------------------------
/**
    This method renders the current view into the render target. This method
    must be called sometimes after UpdateVisibilityLinks()
*/
void
View::Render()
{
    n_assert(this->frameShader.isvalid());      
    n_assert(this->camera.isvalid());

    LightServer* lightServer = LightServer::Instance();
    ShadowServer* shadowServer = ShadowServer::Instance();
    lightServer->BeginFrame(this->camera);
    shadowServer->BeginFrame(this->camera);

    // resolve visible light source
    this->ResolveVisibleLights();

    // draw the shadow pass
    ShadowServer::Instance()->UpdateShadowBuffers();

    // resolve visible ModelNodeInstances
    this->ResolveVisibleModelNodeInstances();

    // render the world...
    TransformDevice* transformDevice = TransformDevice::Instance();
    transformDevice->SetProjTransform(this->camera->GetProjTransform());
    transformDevice->SetViewTransform(this->camera->GetViewTransform());
    transformDevice->ApplyViewSettings();
    this->frameShader->Render();

    // tell light and shadow servers that rendering is finished
    shadowServer->EndFrame();
    lightServer->EndFrame();
}

//------------------------------------------------------------------------------
/**
    Renders a debug visualization. Can be called alone or after Render().
*/
void
View::RenderDebug()
{
    n_assert(this->frameShader.isvalid());

    // setup global transforms...
    TransformDevice* transformDevice = TransformDevice::Instance();
    transformDevice->SetProjTransform(this->camera->GetProjTransform());
    transformDevice->SetViewTransform(this->camera->GetViewTransform());

    // just call OnRenderDebug on all graphics entities visible through our camera
    ShapeRenderer::Instance()->Begin();
    const Array<Ptr<GraphicsEntity> >& visLinks = this->camera->GetLinks(GraphicsEntity::CameraLink);
    IndexT i;
    SizeT num = visLinks.Size();
    for (i = 0; i < num; i++)
    {
        const Ptr<GraphicsEntity>& curEntity = visLinks[i];
        curEntity->OnRenderDebug();
    }
    ShapeRenderer::Instance()->End();
}

//------------------------------------------------------------------------------
/**
    Renders a debug visualization. Must be called inside Begin and End of shaperenderer.
*/
void
View::RenderDebugSimple()
{
    n_assert(this->frameShader.isvalid());
    
    const Array<Ptr<GraphicsEntity> >& visLinks = this->camera->GetLinks(GraphicsEntity::CameraLink);
    IndexT i;
    SizeT num = visLinks.Size();
    for (i = 0; i < num; i++)
    {
        const Ptr<GraphicsEntity>& curEntity = visLinks[i];
        curEntity->OnRenderDebug();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
View::SetCameraEntity(const Ptr<CameraEntity>& newCameraEntity)
{
    // assert if cam is not attached to a stage
    n_assert2(!newCameraEntity.isvalid() || newCameraEntity->IsAttachedToStage(), "Camera has to be attached to a stage first!");
    if (this->camera.isvalid())
    {
        this->camera->OnRemoveFromView(this);
        this->camera = 0;
    }
    if (newCameraEntity.isvalid())
    {
        this->camera = newCameraEntity;
        this->camera->OnAttachToView(this);
    }
}

} // namespace Graphics
