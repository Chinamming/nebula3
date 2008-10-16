#ifndef GRAPHICS_GRAPHICSSERVER_H
#define GRAPHICS_GRAPHICSSERVER_H
//------------------------------------------------------------------------------
/**
    @class Graphics::GraphicsServer
    
    Client-side proxy of the InternalGraphicsServer. Used to create and update 
    Stages and Views.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "graphics/stage.h"
#include "graphics/view.h"

//------------------------------------------------------------------------------
namespace Graphics
{
class GraphicsServer : public Core::RefCounted
{
    __DeclareClass(GraphicsServer);
    __DeclareSingleton(GraphicsServer);
public:
    /// constructor
    GraphicsServer();
    /// destructor
    virtual ~GraphicsServer();

    /// open the graphics server 
    void Open();
    /// close the graphics server
    void Close();
    /// return true if graphics server is open
    bool IsOpen() const;
    /// perform client-side per-frame updates
    void OnFrame();

    /// create a stage
    Ptr<Stage> CreateStage(const Util::StringAtom& name, const Core::Rtti& stageBuilderClass, const Attr::AttributeContainer& stageBuilderAttrs);
    /// discard a stage object
    void DiscardStage(const Ptr<Stage>& stage);
    /// discard all stage objects
    void DiscardAllStages();
    /// return true if a stage exists by name
    bool HasStage(const Util::StringAtom& name) const;
    /// lookup a stage by name
    const Ptr<Stage>& GetStageByName(const Util::StringAtom& name) const;
    /// get all stages
    const Util::Array<Ptr<Stage> >& GetStages() const;

    /// create a view
    Ptr<View> CreateView(const Core::Rtti& viewClass, const Util::StringAtom& viewName, const Util::StringAtom& stageName, const Resources::ResourceId& frameShaderName, bool isDefaultView=false);
    /// discard a view 
    void DiscardView(const Ptr<View>& view);
    /// discard all views
    void DiscardAllViews();
    /// return true if a view exists by name
    bool HasView(const Util::StringAtom& name) const;
    /// lookup a view by name
    const Ptr<View>& GetViewByName(const Util::StringAtom& name) const;
    /// get all view
    const Util::Array<Ptr<View> >& GetViews() const;
    /// get the default view
    const Ptr<View>& GetDefaultView() const;

    /// FIXME: this does not belong here!!! move into dedicated RenderUtil class and make static method!
    Math::line ComputeWorldMouseRay(const Math::float2& mousePos, float length, const Math::matrix44& viewMatrix, const Math::matrix44& invProjMatrix, float nearPlane);

private:
    Util::Array<Ptr<Stage> > stages;                         
    Util::Dictionary<Util::StringAtom, IndexT> stageIndexMap; 
    Util::Array<Ptr<View> > views;
    Util::Dictionary<Util::StringAtom, IndexT> viewIndexMap;
    Ptr<View> defaultView;
    bool isOpen;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
GraphicsServer::IsOpen() const
{
    return this->isOpen;
}

} // namespace Graphics
//------------------------------------------------------------------------------
#endif
