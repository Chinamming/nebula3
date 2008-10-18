//------------------------------------------------------------------------------
//  internalgraphicsserver.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalgraphics/internalgraphicsserver.h"
#include "internalgraphics/internalstage.h"
#include "internalgraphics/internalview.h"
#include "internalgraphics/stagebuilder.h"
#include "resources/resourcemanager.h"
#include "coregraphics/renderdevice.h"
#include "coregraphics/shaderserver.h"
#include "coregraphics/displaydevice.h"
#include "coregraphics/shaperenderer.h"

namespace InternalGraphics
{
__ImplementClass(InternalGraphics::InternalGraphicsServer, 'IGFS', Core::RefCounted);
__ImplementSingleton(InternalGraphics::InternalGraphicsServer);

using namespace Core;
using namespace Util;
using namespace Math;
using namespace Resources;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
InternalGraphicsServer::InternalGraphicsServer() :
    isOpen(false),
    frameCount(0),
    renderDebug(false)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
InternalGraphicsServer::~InternalGraphicsServer()
{
    n_assert(!this->isOpen);
    n_assert(this->stages.IsEmpty());
    n_assert(this->stageIndexMap.IsEmpty());
    n_assert(this->views.IsEmpty());
    n_assert(this->viewIndexMap.IsEmpty());
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
InternalGraphicsServer::Open()
{
    n_assert(!this->isOpen);
    this->isOpen = true;
    
    // get time shader variable from shader server
    this->timeShaderVar = ShaderServer::Instance()->GetSharedVariableBySemantic(ShaderVariable::Semantic("Time"));
    n_assert(this->timeShaderVar.isvalid());
}

//------------------------------------------------------------------------------
/**
*/
void
InternalGraphicsServer::Close()
{
    n_assert(this->isOpen);
    this->timeShaderVar = 0;
    this->DiscardAllViews();
    this->DiscardAllStages();
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
Ptr<InternalStage>
InternalGraphicsServer::CreateStage(const StringAtom& stageName, const Ptr<StageBuilder>& stageBuilder)
{
    n_assert(this->isOpen);
    n_assert(!this->stageIndexMap.Contains(stageName));

    Ptr<InternalStage> newStage = InternalStage::Create();
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
InternalGraphicsServer::DiscardStage(const Ptr<InternalStage>& stage)
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
InternalGraphicsServer::DiscardAllStages()
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
InternalGraphicsServer::HasStage(const StringAtom& stageName) const
{
    return this->stageIndexMap.Contains(stageName);
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<InternalStage>&
InternalGraphicsServer::GetStageByName(const StringAtom& stageName) const
{
    return this->stages[this->stageIndexMap[stageName]];
}

//------------------------------------------------------------------------------
/**
*/
const Array<Ptr<InternalStage> >&
InternalGraphicsServer::GetStages() const
{
    return this->stages;
}

//------------------------------------------------------------------------------
/**
*/
Ptr<InternalView>
InternalGraphicsServer::CreateView(const Core::Rtti& viewClass, const StringAtom& viewName, bool isDefaultView)
{
    n_assert(!this->viewIndexMap.Contains(viewName));
    n_assert(viewClass.IsDerivedFrom(InternalView::RTTI));

    Ptr<InternalView> newView = (InternalView*) viewClass.Create();
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
InternalGraphicsServer::DiscardView(const Ptr<InternalView>& view)
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
InternalGraphicsServer::DiscardAllViews()
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
InternalGraphicsServer::HasView(const StringAtom& viewName) const
{
    return this->viewIndexMap.Contains(viewName);
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<InternalView>&
InternalGraphicsServer::GetViewByName(const StringAtom& viewName) const
{
    return this->views[this->viewIndexMap[viewName]];
}

//------------------------------------------------------------------------------
/**
*/
const Array<Ptr<InternalView> >&
InternalGraphicsServer::GetViews() const
{
    return this->views;
}

//------------------------------------------------------------------------------
/**
*/
void
InternalGraphicsServer::SetDefaultView(const Ptr<InternalView>& defView)
{
    this->defaultView = defView;
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<InternalView>&
InternalGraphicsServer::GetDefaultView() const
{
    return this->defaultView;
}

//------------------------------------------------------------------------------
/**
*/
void
InternalGraphicsServer::OnFrame(Timing::Time curTime)
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
            const Ptr<InternalStage>& defaultStage = this->defaultView->GetStage();

            // update the view's stage, this will happen only once
            // per frame, regardless of how many views are attached to the stage
            defaultStage->UpdateEntities(curTime, this->frameCount);

            // update visibility from the default view's camera
            this->defaultView->UpdateVisibilityLinks();

            // update light linking for visible lights
            defaultStage->UpdateLightLinks();

            // update time
            this->timeShaderVar->SetFloat((float)curTime);

            // finally render the view
            this->defaultView->Render();

            // render bounding boxes if debug rendering is enabled
            if (this->renderDebug)
            {
                this->defaultView->RenderDebug();
            }
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

} // namespace InternalGraphics
