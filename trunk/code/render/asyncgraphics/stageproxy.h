#pragma once
#ifndef ASYNCGRAPHICS_STAGEPROXY_H
#define ASYNCGRAPHICS_STAGEPROXY_H
//------------------------------------------------------------------------------
/**
    class AsyncGraphics::StageProxy
    
    A client-side proxy of a Graphics::Stage in the AsyncGraphics
    subsystem. The StageProxy offers a friendly frontend to the
    client thread, and communicates with its server-side 
    Stage object through the AsyncGraphicsInterface method. There
    is a 1:1 relationship between the server-side Stage and the
    client-side StageProxy, thus the StageProxy may safely store
    read-only information on the client side without synchronizing
    with its Stage.

    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/rtti.h"
#include "util/stringatom.h"
#include "attr/attributecontainer.h"
#include "asyncgraphics/handle.h"
#include "asyncgraphics/graphicsentityproxy.h"

//------------------------------------------------------------------------------
namespace AsyncGraphics
{
class StageProxy : public Core::RefCounted
{
    DeclareClass(StageProxy);
public:
    /// constructor
    StageProxy();
    /// destructor
    virtual ~StageProxy();

    /// return true if the StageProxy is valid
    bool IsValid() const;
    /// get name of stage proxy
    const Util::StringAtom& GetName() const;
    /// get StageBuilder type
    const Core::Rtti& GetStageBuilderClass() const;
    /// get StageBuilder attributes
    const Attr::AttributeContainer& GetStageBuilderAttributes() const;

    /// attach a graphics entity proxy
    void AttachEntityProxy(const Ptr<GraphicsEntityProxy>& entityProxy);
    /// remove a graphics entity proxy
    void RemoveEntityProxy(const Ptr<GraphicsEntityProxy>& entityProxy);
    /// get an array of all entity proxies attached to the stage
    const Util::Array<Ptr<GraphicsEntityProxy> >& GetEntityProxies() const;
    /// get an array of entity proxies filtered by type
    const Util::Array<Ptr<GraphicsEntityProxy> >& GetEntityProxiesByType(Graphics::GraphicsEntity::Type type) const;
    
private:
    friend class GraphicsServerProxy;

    /// set name of stage proxy
    void SetName(const Util::StringAtom& name);
    /// set StageBuilder type
    void SetStageBuilderClass(const Core::Rtti& stageBuilderClass);
    /// set StageBuilder attributes
    void SetStageBuilderAttributes(const Attr::AttributeContainer& stageBuilderAttrs);
    /// setup the stage proxy (waits for completion)
    void Setup();
    /// discard the stage proxy (waits for completion)
    void Discard();
    /// call this method per-frame
    void OnFrame();

    Util::StringAtom name;
    const Core::Rtti* stageBuilderClass;
    Attr::AttributeContainer stageBuilderAttrs;
    AsyncGraphics::Handle stageHandle;
    Util::Array<Ptr<GraphicsEntityProxy> > entityProxies;
    Util::Array<Ptr<GraphicsEntityProxy> > entityProxiesByType[Graphics::GraphicsEntity::NumTypes];
    Util::Array<Ptr<GraphicsEntityProxy> > pendingEntityProxies;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
StageProxy::IsValid() const
{
    return (0 != this->stageHandle);
}

//------------------------------------------------------------------------------
/**
*/
inline void
StageProxy::SetName(const Util::StringAtom& n)
{
    this->name = n;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
StageProxy::GetName() const
{
    return this->name;
}

//------------------------------------------------------------------------------
/**
*/
inline void
StageProxy::SetStageBuilderClass(const Core::Rtti& cl)
{
    this->stageBuilderClass = &cl;
}

//------------------------------------------------------------------------------
/**
*/
inline const Core::Rtti&
StageProxy::GetStageBuilderClass() const
{
    n_assert(0 != this->stageBuilderClass);
    return *(this->stageBuilderClass);
}

//------------------------------------------------------------------------------
/**
*/
inline void
StageProxy::SetStageBuilderAttributes(const Attr::AttributeContainer& attrs)
{
    this->stageBuilderAttrs = attrs;
}

//------------------------------------------------------------------------------
/**
*/
inline const Attr::AttributeContainer&
StageProxy::GetStageBuilderAttributes() const
{
    return this->stageBuilderAttrs;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<GraphicsEntityProxy> >& 
StageProxy::GetEntityProxies() const
{
    return this->entityProxies;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<GraphicsEntityProxy> >& 
StageProxy::GetEntityProxiesByType(Graphics::GraphicsEntity::Type entityType) const
{
    n_assert((entityType >= 0) && (entityType < Graphics::GraphicsEntity::NumTypes));
    return this->entityProxiesByType[entityType];
}

} // namespace AsyncGraphics
//------------------------------------------------------------------------------
#endif
