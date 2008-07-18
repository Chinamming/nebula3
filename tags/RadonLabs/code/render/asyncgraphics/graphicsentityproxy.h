#pragma once
#ifndef ASYNCGRAPHICS_GRAPHICSENTITYPROXY
#define ASYNCGRAPHICS_GRAPHICSENTITYPROXY
//------------------------------------------------------------------------------
/**
    @class AsyncGraphics::GraphicsEntityProxy
    
    Client-side proxy of a GraphicsEntity in the AsyncGraphics subsystem.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "graphics/graphicsentity.h"
#include "util/queue.h"
#include "messaging/message.h"
#include "asyncgraphics/protocol.h"

//------------------------------------------------------------------------------
namespace AsyncGraphics
{
class StageProxy;

class GraphicsEntityProxy : public Core::RefCounted
{
    DeclareClass(GraphicsEntityProxy);
public:
    /// constructor
    GraphicsEntityProxy();
    /// destructor
    virtual ~GraphicsEntityProxy();
        
    /// return true if entity is valid (is attached to a stage)
    bool IsValid() const;
    /// get the entity type
    Graphics::GraphicsEntity::Type GetType() const;
    /// set the entity's world space transform
    void SetTransform(const Math::matrix44& m);
    /// get the entity's world space transform
    const Math::matrix44& GetTransform() const;
    /// set the entity's visibility
    void SetVisible(bool b);
    /// return true if entity is set to visible
    bool IsVisible() const;
    /// get the stage proxy this entity is attached to
    const Ptr<StageProxy>& GetStageProxy() const;
    /// return true if entity is attached to stage
    bool IsAttachedToStageProxy() const;

protected:
    friend class StageProxy;

    /// set graphics entity type, called from constructor of subclass
    void SetType(Graphics::GraphicsEntity::Type t);
    /// called by stage when entity should setup itself
    virtual void Setup(const Ptr<StageProxy>& stage);
    /// called by stage when entity should discard itself
    virtual void Discard();
    /// validate entity handle
    void ValidateEntityHandle(bool wait=false); 
    /// send off a specific create message from the subclass
    void SendCreateMsg(const Ptr<CreateGraphicsEntity>& msg);
    /// send a message to the server-side graphics entity
    void SendMsg(const Ptr<GraphicsEntityMsg>& msg);
    /// test if the entity handle is valid
    bool IsEntityHandleValid() const;
    /// called when transform matrix changed
    virtual void OnTransformChanged();

    Math::matrix44 transform;
    Ptr<StageProxy> stageProxy;
    Graphics::GraphicsEntity::Type type;
    bool isVisible;
    Handle graphicsEntityHandle;    
    Ptr<CreateGraphicsEntity> pendingCreateMsg;
    Util::Array<Ptr<GraphicsEntityMsg> > pendingMessages; 
}; 

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
GraphicsEntityProxy::GetTransform() const
{
    return this->transform;
}

//------------------------------------------------------------------------------
/**
*/
inline void
GraphicsEntityProxy::SetType(Graphics::GraphicsEntity::Type t)
{
    this->type = t;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
GraphicsEntityProxy::IsEntityHandleValid() const
{
    return (0 != this->graphicsEntityHandle);
}

//------------------------------------------------------------------------------
/**
*/
inline Graphics::GraphicsEntity::Type
GraphicsEntityProxy::GetType() const
{
    return this->type;
}

} // namespace AsyncGraphics
//------------------------------------------------------------------------------
#endif
    
