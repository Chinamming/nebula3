#pragma once
#ifndef GRAPHICS_GRAPHICSSERVER_H
#define GRAPHICS_GRAPHICSSERVER_H
//------------------------------------------------------------------------------
/**
    @class Graphics::GraphicsServer
    
    The graphics server maintains a the "graphics world" consisting of 
    one or more "stages" and one or more "views" which are attached to
    the stages.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "util/stringatom.h"
#include "util/array.h"
#include "math/line.h"
#include "math/float2.h"
#include "timing/time.h"

//------------------------------------------------------------------------------
namespace Graphics
{
class Stage;
class View;
class StageBuilder;
    
class GraphicsServer : public Core::RefCounted
{
    DeclareClass(GraphicsServer);
    DeclareSingleton(GraphicsServer);
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
    
    /// create a stage object
    Ptr<Stage> CreateStage(const Util::StringAtom& name, const Ptr<StageBuilder>& stageBuilder);
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

    /// create a view object
    Ptr<View> CreateView(const Core::Rtti& viewClass, const Util::StringAtom& name, bool isDefaultView=false);
    /// discard a view object
    void DiscardView(const Ptr<View>& view);
    /// discard all view objects
    void DiscardAllViews();
    /// return true if a view exists by name
    bool HasView(const Util::StringAtom& name) const;
    /// lookup a view by name
    const Ptr<View>& GetViewByName(const Util::StringAtom& name) const;
    /// get all views
    const Util::Array<Ptr<View> >& GetViews() const;
    /// set the default view
    void SetDefaultView(const Ptr<View>& defView);
    /// get the default view
    const Ptr<View>& GetDefaultView() const;
    
    /// get unique frame counter, updated in OnFrame()
    IndexT GetFrameCount() const;
    /// call per-frame, this renders the default view
    void OnFrame(Timing::Time curTime);

    /// FIXME: this does not belong here!!! move into dedicated RenderUtil class and make static method!
    Math::line ComputeWorldMouseRay(const Math::float2& mousePos, float length, const Math::matrix44& viewMatrix, const Math::matrix44& invProjMatrix, float nearPlane);

private:
    IndexT frameCount;
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

//------------------------------------------------------------------------------
/**
*/
inline IndexT
GraphicsServer::GetFrameCount() const
{
    return this->frameCount;
}

} // namespace Graphics
//------------------------------------------------------------------------------
#endif

