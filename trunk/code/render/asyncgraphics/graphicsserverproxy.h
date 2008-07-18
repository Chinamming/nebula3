#ifndef ASYNCGRAPHICS_GRAPHICSSERVERPROXY_H
#define ASYNCGRAPHICS_GRAPHICSSERVERPROXY_H
//------------------------------------------------------------------------------
/**
    @class AsyncGraphics::GraphicsServerProxy
    
    Client-side proxy of the GraphicsServer. Used to create and update 
    StageProxies and ViewProxies.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "asyncgraphics/stageproxy.h"
#include "asyncgraphics/viewproxy.h"

//------------------------------------------------------------------------------
namespace AsyncGraphics
{
class GraphicsServerProxy : public Core::RefCounted
{
    DeclareClass(GraphicsServerProxy);
    DeclareSingleton(GraphicsServerProxy);
public:
    /// constructor
    GraphicsServerProxy();
    /// destructor
    virtual ~GraphicsServerProxy();

    /// open the graphics server proxy
    void Open();
    /// close the graphics server proxy
    void Close();
    /// return true if graphics server is open
    bool IsOpen() const;
    /// perform per-frame updates
    void OnFrame();

    /// create a stage proxy
    Ptr<StageProxy> CreateStageProxy(const Util::StringAtom& name, const Core::Rtti& stageBuilderClass, const Attr::AttributeContainer& stageBuilderAttrs);
    /// discard a stage proxy object
    void DiscardStageProxy(const Ptr<StageProxy>& stageProxy);
    /// discard all stage proxy objects
    void DiscardAllStageProxies();
    /// return true if a stage proxy exists by name
    bool HasStageProxy(const Util::StringAtom& name) const;
    /// lookup a stage proxy by name
    const Ptr<StageProxy>& GetStageProxyByName(const Util::StringAtom& name) const;
    /// get all stage proxies
    const Util::Array<Ptr<StageProxy> >& GetStageProxies() const;

    /// create a view proxy
    Ptr<ViewProxy> CreateViewProxy(const Core::Rtti& viewClass, const Util::StringAtom& viewName, const Util::StringAtom& stageName, const Resources::ResourceId& frameShaderName, bool isDefaultView=false);
    /// discard a view proxy
    void DiscardViewProxy(const Ptr<ViewProxy>& view);
    /// discard all view proxies
    void DiscardAllViewProxies();
    /// return true if a view proxy exists by name
    bool HasViewProxy(const Util::StringAtom& name) const;
    /// lookup a view proxy by name
    const Ptr<ViewProxy>& GetViewProxyByName(const Util::StringAtom& name) const;
    /// get all view proxies
    const Util::Array<Ptr<ViewProxy> >& GetViewProxies() const;
    /// get the default view proxy
    const Ptr<ViewProxy>& GetDefaultViewProxy() const;

private:
    Util::Array<Ptr<StageProxy> > stageProxies;                         
    Util::Dictionary<Util::StringAtom, IndexT> stageProxyIndexMap; 
    Util::Array<Ptr<ViewProxy> > viewProxies;
    Util::Dictionary<Util::StringAtom, IndexT> viewProxyIndexMap;
    Ptr<ViewProxy> defaultViewProxy;
    bool isOpen;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
GraphicsServerProxy::IsOpen() const
{
    return this->isOpen;
}

} // namespace AsyncGraphics
//------------------------------------------------------------------------------
#endif
