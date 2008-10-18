#pragma once
#ifndef INTERNALGRAPHICS_INTERNALGRAPHICSSERVER_H
#define INTERNALGRAPHICS_INTERNALGRAPHICSSERVER_H
//------------------------------------------------------------------------------
/**
    @class InternalGraphics::InternalGraphicsServer
    
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
#include "coregraphics/shadervariable.h"

//------------------------------------------------------------------------------
namespace InternalGraphics
{
class InternalStage;
class InternalView;
class StageBuilder;
    
class InternalGraphicsServer : public Core::RefCounted
{
    __DeclareClass(InternalGraphicsServer);
    __DeclareSingleton(InternalGraphicsServer);
public:
    /// constructor
    InternalGraphicsServer();
    /// destructor
    virtual ~InternalGraphicsServer();
    
    /// open the graphics server
    void Open();
    /// close the graphics server
    void Close();
    /// return true if graphics server is open
    bool IsOpen() const;
    
    /// create a stage object
    Ptr<InternalStage> CreateStage(const Util::StringAtom& name, const Ptr<StageBuilder>& stageBuilder);
    /// discard a stage object
    void DiscardStage(const Ptr<InternalStage>& stage);
    /// discard all stage objects
    void DiscardAllStages();
    /// return true if a stage exists by name
    bool HasStage(const Util::StringAtom& name) const;
    /// lookup a stage by name
    const Ptr<InternalStage>& GetStageByName(const Util::StringAtom& name) const;
    /// get all stages
    const Util::Array<Ptr<InternalStage> >& GetStages() const;

    /// create a view object
    Ptr<InternalView> CreateView(const Core::Rtti& viewClass, const Util::StringAtom& name, bool isDefaultView=false);
    /// discard a view object
    void DiscardView(const Ptr<InternalView>& view);
    /// discard all view objects
    void DiscardAllViews();
    /// return true if a view exists by name
    bool HasView(const Util::StringAtom& name) const;
    /// lookup a view by name
    const Ptr<InternalView>& GetViewByName(const Util::StringAtom& name) const;
    /// get all views
    const Util::Array<Ptr<InternalView> >& GetViews() const;
    /// set the default view
    void SetDefaultView(const Ptr<InternalView>& defView);
    /// get the default view
    const Ptr<InternalView>& GetDefaultView() const;
    
    /// get unique frame counter, updated in OnFrame()
    IndexT GetFrameCount() const;
    /// call per-frame, this renders the default view
    void OnFrame(Timing::Time curTime);
    /// set render debug flag
    void SetRenderDebug(bool b);
    /// get render debug flag
    bool GetRenderDebug() const;

private:
    IndexT frameCount;
    Util::Array<Ptr<InternalStage> > stages;                         
    Util::Dictionary<Util::StringAtom, IndexT> stageIndexMap; 
    Util::Array<Ptr<InternalView> > views;
    Util::Dictionary<Util::StringAtom, IndexT> viewIndexMap;
    Ptr<InternalView> defaultView;
    Ptr<CoreGraphics::ShaderVariable> timeShaderVar;
    bool isOpen;
    bool renderDebug;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
InternalGraphicsServer::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
InternalGraphicsServer::GetFrameCount() const
{
    return this->frameCount;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
InternalGraphicsServer::SetRenderDebug(bool b)
{
    this->renderDebug = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
InternalGraphicsServer::GetRenderDebug() const
{
    return this->renderDebug;
}

} // namespace Graphics
//------------------------------------------------------------------------------
#endif

