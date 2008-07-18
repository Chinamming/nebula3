//------------------------------------------------------------------------------
//  graphicsserverproxy.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "asyncgraphics/graphicsserverproxy.h"

namespace AsyncGraphics
{
ImplementClass(AsyncGraphics::GraphicsServerProxy, 'AGSV', Core::RefCounted);
ImplementSingleton(AsyncGraphics::GraphicsServerProxy);

using namespace Util;

//------------------------------------------------------------------------------
/**
*/
GraphicsServerProxy::GraphicsServerProxy() :
    isOpen(false)
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
GraphicsServerProxy::~GraphicsServerProxy()
{
    n_assert(!this->isOpen);
    n_assert(this->stageProxies.IsEmpty());
    n_assert(this->stageProxyIndexMap.IsEmpty());
    n_assert(this->viewProxies.IsEmpty());
    n_assert(this->viewProxyIndexMap.IsEmpty());
    DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsServerProxy::Open()
{
    n_assert(!this->isOpen);
    this->isOpen = true;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsServerProxy::Close()
{
    n_assert(this->isOpen);
    this->DiscardAllViewProxies();
    this->DiscardAllStageProxies();
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
Ptr<StageProxy>
GraphicsServerProxy::CreateStageProxy(const Util::StringAtom& name, const Core::Rtti& stageBuilderClass, const Attr::AttributeContainer& stageBuilderAttrs)
{
    n_assert(this->isOpen);
    n_assert(!this->stageProxyIndexMap.Contains(name));

    Ptr<StageProxy> stageProxy = StageProxy::Create();
    stageProxy->SetName(name);
    stageProxy->SetStageBuilderClass(stageBuilderClass);
    stageProxy->SetStageBuilderAttributes(stageBuilderAttrs);
    stageProxy->Setup();

    this->stageProxies.Append(stageProxy);
    this->stageProxyIndexMap.Add(name, this->stageProxies.Size() - 1);

    return stageProxy;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsServerProxy::DiscardStageProxy(const Ptr<StageProxy>& stageProxy)
{
    n_assert(this->stageProxyIndexMap.Contains(stageProxy->GetName()));
    this->stageProxies.EraseIndex(this->stageProxyIndexMap[stageProxy->GetName()]);
    this->stageProxyIndexMap.Erase(stageProxy->GetName());
    stageProxy->Discard();
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsServerProxy::DiscardAllStageProxies()
{
    IndexT i;
    for (i = 0; i < this->stageProxies.Size(); i++)
    {
        this->stageProxies[i]->Discard();
    }
    this->stageProxies.Clear();
    this->stageProxyIndexMap.Clear();
}

//------------------------------------------------------------------------------
/**
*/
bool
GraphicsServerProxy::HasStageProxy(const StringAtom& stageName) const
{
    return this->stageProxyIndexMap.Contains(stageName);
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<StageProxy>&
GraphicsServerProxy::GetStageProxyByName(const StringAtom& stageName) const
{
    return this->stageProxies[this->stageProxyIndexMap[stageName]];
}

//------------------------------------------------------------------------------
/**
*/
const Array<Ptr<StageProxy> >&
GraphicsServerProxy::GetStageProxies() const
{
    return this->stageProxies;
}

//------------------------------------------------------------------------------
/**
*/
Ptr<ViewProxy>
GraphicsServerProxy::CreateViewProxy(const Core::Rtti& viewClass, const Util::StringAtom& viewName, const Util::StringAtom& stageName, const Resources::ResourceId& frameShaderName, bool isDefaultView)
{
    n_assert(this->isOpen);
    n_assert(!this->viewProxyIndexMap.Contains(viewName));

    Ptr<ViewProxy> viewProxy = ViewProxy::Create();
    viewProxy->SetName(viewName);
    viewProxy->SetViewClass(viewClass);
    viewProxy->SetStageName(stageName);
    viewProxy->SetFrameShaderName(frameShaderName);
    viewProxy->SetDefaultView(isDefaultView);
    viewProxy->Setup();

    this->viewProxies.Append(viewProxy);
    this->viewProxyIndexMap.Add(viewName, this->viewProxies.Size() - 1);
    if (isDefaultView)
    {
        this->defaultViewProxy = viewProxy;
    }
    return viewProxy;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsServerProxy::DiscardViewProxy(const Ptr<ViewProxy>& viewProxy)
{
    n_assert(this->isOpen);
    n_assert(this->viewProxyIndexMap.Contains(viewProxy->GetName()));

    this->viewProxies.EraseIndex(this->viewProxyIndexMap[viewProxy->GetName()]);
    this->viewProxyIndexMap.Erase(viewProxy->GetName());
    if (viewProxy == this->defaultViewProxy)
    {
        this->defaultViewProxy = 0;
    }
    viewProxy->Discard();
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsServerProxy::DiscardAllViewProxies()
{
    n_assert(this->isOpen);
    IndexT i;
    for (i = 0; i < this->viewProxies.Size(); i++)
    {
        this->viewProxies[i]->Discard();
    }
    this->viewProxies.Clear();
    this->viewProxyIndexMap.Clear();
    this->defaultViewProxy = 0;
}

//------------------------------------------------------------------------------
/**
*/
bool
GraphicsServerProxy::HasViewProxy(const StringAtom& viewName) const
{
    return this->viewProxyIndexMap.Contains(viewName);
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<ViewProxy>&
GraphicsServerProxy::GetViewProxyByName(const StringAtom& viewName) const
{
    return this->viewProxies[this->viewProxyIndexMap[viewName]];
}

//------------------------------------------------------------------------------
/**
*/
const Array<Ptr<ViewProxy> >&
GraphicsServerProxy::GetViewProxies() const
{
    return this->viewProxies;
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<ViewProxy>&
GraphicsServerProxy::GetDefaultViewProxy() const
{
    return this->defaultViewProxy;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsServerProxy::OnFrame()
{
    // need to call OnFrame() on all stages
    IndexT i;
    for (i = 0; i < this->stageProxies.Size(); i++)
    {
        this->stageProxies[i]->OnFrame();
    }
}

} // namespace AsyncGraphics
