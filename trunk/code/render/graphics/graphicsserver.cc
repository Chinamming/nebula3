//------------------------------------------------------------------------------
//  graphicsserver.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/graphicsserver.h"

namespace Graphics
{
__ImplementClass(Graphics::GraphicsServer, 'GSRV', Core::RefCounted);
__ImplementSingleton(Graphics::GraphicsServer);

using namespace Util;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
GraphicsServer::GraphicsServer() :
    isOpen(false)
{
    __ConstructSingleton;
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
    __DestructSingleton;
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
GraphicsServer::CreateStage(const Util::StringAtom& name, const Core::Rtti& stageBuilderClass, const Attr::AttributeContainer& stageBuilderAttrs)
{
    n_assert(this->isOpen);
    n_assert(!this->stageIndexMap.Contains(name));

    Ptr<Stage> stage = Stage::Create();
    stage->SetName(name);
    stage->SetStageBuilderClass(stageBuilderClass);
    stage->SetStageBuilderAttributes(stageBuilderAttrs);
    stage->Setup();

    this->stages.Append(stage);
    this->stageIndexMap.Add(name, this->stages.Size() - 1);

    return stage;
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
    stage->Discard();
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsServer::DiscardAllStages()
{
    IndexT i;
    for (i = 0; i < this->stages.Size(); i++)
    {
        this->stages[i]->Discard();
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
GraphicsServer::CreateView(const Core::Rtti& viewClass, const Util::StringAtom& viewName, const Util::StringAtom& stageName, const Resources::ResourceId& frameShaderName, bool isDefaultView)
{
    n_assert(this->isOpen);
    n_assert(!this->viewIndexMap.Contains(viewName));

    Ptr<View> view = View::Create();
    view->SetName(viewName);
    view->SetViewClass(viewClass);
    view->SetStageName(stageName);
    view->SetFrameShaderName(frameShaderName);
    view->SetDefaultView(isDefaultView);
    view->Setup();

    this->views.Append(view);
    this->viewIndexMap.Add(viewName, this->views.Size() - 1);
    if (isDefaultView)
    {
        this->defaultView = view;
    }
    return view;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsServer::DiscardView(const Ptr<View>& view)
{
    n_assert(this->isOpen);
    n_assert(this->viewIndexMap.Contains(view->GetName()));

    this->views.EraseIndex(this->viewIndexMap[view->GetName()]);
    this->viewIndexMap.Erase(view->GetName());
    if (view == this->defaultView)
    {
        this->defaultView = 0;
    }
    view->Discard();
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsServer::DiscardAllViews()
{
    n_assert(this->isOpen);
    IndexT i;
    for (i = 0; i < this->views.Size(); i++)
    {
        this->views[i]->Discard();
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
const Ptr<View>&
GraphicsServer::GetDefaultView() const
{
    return this->defaultView;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsServer::OnFrame()
{
    IndexT i;
    for (i = 0; i < this->stages.Size(); i++)
    {
        this->stages[i]->OnFrame();
    }
}

//------------------------------------------------------------------------------
/**
    Utility function which computes a ray in world space between the eye
    and the current mouse position on the near plane.

    FIXME: this doesn't belong here!
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
    vector worldMouseDir = worldMousePos - point(viewMatrix.get_position());
    worldMouseDir = vector::normalize(worldMouseDir);
    worldMouseDir *= length;

    return line(worldMousePos, worldMousePos + worldMouseDir);
}

} // namespace Graphics
