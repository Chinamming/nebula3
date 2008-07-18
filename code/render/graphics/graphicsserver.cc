//------------------------------------------------------------------------------
//  graphicsserver.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/graphicsserver.h"
#include "graphics/stage.h"
#include "graphics/view.h"
#include "graphics/stagebuilder.h"
#include "resources/resourcemanager.h"
#include "coregraphics/renderdevice.h"
#include "coregraphics/displaydevice.h"

namespace Graphics
{
ImplementClass(Graphics::GraphicsServer, 'GFXS', Core::RefCounted);
ImplementSingleton(Graphics::GraphicsServer);

using namespace Core;
using namespace Util;
using namespace Math;
using namespace Resources;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
GraphicsServer::GraphicsServer() :
    isOpen(false),
    frameCount(0)
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
GraphicsServer::~GraphicsServer()
{
    n_assert(!this->isOpen);
    n_assert(this->stages.IsEmpty());
    n_assert(this->stageIndexMap.IsEmpty());
    n_assert(this->views.IsEmpty());
    n_assert(this->viewIndexMap.IsEmpty());
    DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsServer::Open()
{
    n_assert(!this->isOpen);
    this->isOpen = true;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsServer::Close()
{
    n_assert(this->isOpen);
    this->DiscardAllViews();
    this->DiscardAllStages();
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
Ptr<Stage>
GraphicsServer::CreateStage(const StringAtom& stageName, const Ptr<StageBuilder>& stageBuilder)
{
    n_assert(this->isOpen);
    n_assert(!this->stageIndexMap.Contains(stageName));

    Ptr<Stage> newStage = Stage::Create();
    newStage->SetName(stageName);
    newStage->SetStageBuilder(stageBuilder);
    newStage->OnAttachToServer();

    this->stages.Append(newStage);
    this->stageIndexMap.Add(stageName, this->stages.Size() - 1);

    return newStage;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsServer::DiscardStage(const Ptr<Stage>& stage)
{
    n_assert(this->stageIndexMap.Contains(stage->GetName()));
    this->stages.EraseIndex(this->stageIndexMap[stage->GetName()]);
    this->stageIndexMap.Erase(stage->GetName());
    stage->OnRemoveFromServer();
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsServer::DiscardAllStages()
{
    IndexT i;
    SizeT num = this->stages.Size();
    for (i = 0; i < num; i++)
    {
        this->stages[i]->OnRemoveFromServer();
    }
    this->stages.Clear();
    this->stageIndexMap.Clear();
}

//------------------------------------------------------------------------------
/**
*/
bool
GraphicsServer::HasStage(const StringAtom& stageName) const
{
    return this->stageIndexMap.Contains(stageName);
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<Stage>&
GraphicsServer::GetStageByName(const StringAtom& stageName) const
{
    return this->stages[this->stageIndexMap[stageName]];
}

//------------------------------------------------------------------------------
/**
*/
const Array<Ptr<Stage> >&
GraphicsServer::GetStages() const
{
    return this->stages;
}

//------------------------------------------------------------------------------
/**
*/
Ptr<View>
GraphicsServer::CreateView(const Core::Rtti& viewClass, const StringAtom& viewName, bool isDefaultView)
{
    n_assert(!this->viewIndexMap.Contains(viewName));
    n_assert(viewClass.IsDerivedFrom(View::RTTI));

    Ptr<View> newView = (View*) viewClass.Create();
    newView->SetName(viewName);
    newView->OnAttachToServer();

    this->views.Append(newView);
    this->viewIndexMap.Add(viewName, this->views.Size() - 1);
    if (isDefaultView)
    {
        this->SetDefaultView(newView);
    }
    return newView;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsServer::DiscardView(const Ptr<View>& view)
{
    n_assert(this->viewIndexMap.Contains(view->GetName()));
    this->views.EraseIndex(this->viewIndexMap[view->GetName()]);
    this->viewIndexMap.Erase(view->GetName());
    if (view == this->defaultView)
    {
        this->defaultView = 0;
    }
    view->OnRemoveFromServer();
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsServer::DiscardAllViews()
{
    IndexT i;
    SizeT num = this->views.Size();
    for (i = 0; i < num; i++)
    {
        this->views[i]->OnRemoveFromServer();
    }
    this->views.Clear();
    this->viewIndexMap.Clear();
    this->defaultView = 0;
}

//------------------------------------------------------------------------------
/**
*/
bool
GraphicsServer::HasView(const StringAtom& viewName) const
{
    return this->viewIndexMap.Contains(viewName);
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<View>&
GraphicsServer::GetViewByName(const StringAtom& viewName) const
{
    return this->views[this->viewIndexMap[viewName]];
}

//------------------------------------------------------------------------------
/**
*/
const Array<Ptr<View> >&
GraphicsServer::GetViews() const
{
    return this->views;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsServer::SetDefaultView(const Ptr<View>& defView)
{
    this->defaultView = defView;
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<View>&
GraphicsServer::GetDefaultView() const
{
    return this->defaultView;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsServer::OnFrame(Timing::Time curTime)
{
    RenderDevice* renderDevice = RenderDevice::Instance();
    DisplayDevice* displayDevice = DisplayDevice::Instance();

    // call pre-render update on resource manager
    ResourceManager::Instance()->Prepare();

    // process window messages
    displayDevice->ProcessWindowMessages();

    // start rendering
    if (renderDevice->BeginFrame())
    {
        // render the default view
        if (this->defaultView.isvalid() && this->defaultView->GetCameraEntity().isvalid())
        {
            const Ptr<Stage> defaultStage = this->defaultView->GetStage();

            // update the view's stage, this will happen only once
            // per frame, regardless of how many views are attached to the stage
            defaultStage->UpdateEntities(curTime, this->frameCount);

            // update visibility from the default view's camera
            this->defaultView->UpdateVisibilityLinks();

            // update light linking for visible lights
            defaultStage->UpdateLightLinks();

            // finally render the view
            this->defaultView->Render();
        }
        else
        {
            // hmm, no default view set, or default view doesn't have a valid
            // camera yet, just render an empty frame
            renderDevice->BeginPass(renderDevice->GetDefaultRenderTarget(), 0);
            renderDevice->EndPass();
        }
        renderDevice->EndFrame();
        renderDevice->Present();
    }
    
    // call post-render update on Resource Manager
    ResourceManager::Instance()->Update();

    // if we're running in windowed mode, give up time-slice
    if (!displayDevice->IsFullscreen())
    {
        Timing::Sleep(0.0);
    }

    // update the frame count
    this->frameCount++;
}

//------------------------------------------------------------------------------
/**
    Utility function which computes a ray in world space between the eye
    and the current mouse position on the near plane.
*/
line
GraphicsServer::ComputeWorldMouseRay(const float2& mousePos, float length, const matrix44& viewMatrix, const matrix44& invProjMatrix, float nearPlane)
{
    // Compute mouse position in world coordinates.
    point screenCoord3D((mousePos.x() - 0.5f) * 2.0f, (mousePos.y() - 0.5f) * 2.0f, 1.0f);
    vector viewCoord = vector::transform(screenCoord3D, invProjMatrix);
    point localMousePos = viewCoord * nearPlane * 1.1f;
    localMousePos.y() = -1 * localMousePos.y();
    point worldMousePos = vector::transform(localMousePos, viewMatrix);
    vector worldMouseDir = worldMousePos - point(viewMatrix.getpos_component());
    worldMouseDir = vector::normalize(worldMouseDir);
    worldMouseDir *= length;

    return line(worldMousePos, worldMousePos + worldMouseDir);
}

} // namespace Graphics