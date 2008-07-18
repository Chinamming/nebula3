#pragma once
#ifndef GRAPHICS_VIEW_H
#define GRAPHICS_VIEW_H
//------------------------------------------------------------------------------
/**
    @class Graphics::View
  
    A graphics View is used to render a Stage through a CameraEntity into a 
    RenderTarget. Any number of views can be associated with the same Stage.
    Views may depend on other views. When a View is rendered, it will
    first ask the Views it depends on to render themselves. Subclasses of
    View may implement their own rendering strategies.
    
    (C) 2007 Radon Labs GmbH
*/    
#include "core/refcounted.h"
#include "util/stringatom.h"
#include "graphics/stage.h"
#include "graphics/cameraentity.h"
#include "coregraphics/texture.h"
#include "coregraphics/rendertarget.h"
#include "frame/frameshader.h"

//------------------------------------------------------------------------------
namespace Graphics
{
class View : public Core::RefCounted
{
    DeclareClass(View);
public:
    /// constructor
    View();
    /// destructor
    virtual ~View();

    /// return true if currently attached to graphics server
    bool IsAttachedToServer() const;
    /// get human-readable name
    const Util::StringAtom& GetName() const;
    /// set the stage this View is associated with
    void SetStage(const Ptr<Stage>& stage);
    /// get the stage this View is associated with
    const Ptr<Stage>& GetStage() const;
    /// set the CameraEntity this View looks through
    void SetCameraEntity(const Ptr<CameraEntity>& camera);
    /// get the CameraEntity this View looks through
    const Ptr<CameraEntity>& GetCameraEntity() const;
    /// set the render target this view renders to
    void SetRenderTarget(const Ptr<CoreGraphics::RenderTarget>& renderTarget);
    /// get the render target this view renders to
    const Ptr<CoreGraphics::RenderTarget>& GetRenderTarget() const;
    /// set the view's frame shader 
    void SetFrameShader(const Ptr<Frame::FrameShader>& frameShader);
    /// get the view's frame shader
    const Ptr<Frame::FrameShader>& GetFrameShader() const;

    /// add a view which this view depends on
    void AddDependency(const Ptr<View>& view);
    /// get all dependency views
    const Util::Array<Ptr<View> >& GetDependencies() const;

    /// update the visibility links for this view 
    virtual void UpdateVisibilityLinks();
    /// render the view into its render target
    virtual void Render();
    /// render a debug view of the world
    virtual void RenderDebug();
    /// render a debug view of the world, without begin and end shape renderering
    virtual void RenderDebugSimple();

protected:
    friend class GraphicsServer;

    /// set a human-readable name of the view
    void SetName(const Util::StringAtom& name);
    /// called when attached to graphics server
    virtual void OnAttachToServer();
    /// called when detached from graphics server
    virtual void OnRemoveFromServer();
    /// resolve visible lights
    void ResolveVisibleLights();
    /// resolve visibility for optimal batch rendering
    void ResolveVisibleModelNodeInstances();

    bool isAttachedToServer;
    Util::StringAtom name;
    Ptr<Stage> stage;
    Ptr<CameraEntity> camera;
    Ptr<CoreGraphics::RenderTarget> renderTarget;
    Ptr<Frame::FrameShader> frameShader;
    Util::Array<Ptr<View> > dependencies;    
};

//------------------------------------------------------------------------------
/**
*/
inline bool
View::IsAttachedToServer() const
{
    return this->isAttachedToServer;
}

//------------------------------------------------------------------------------
/**
*/
inline void
View::SetName(const Util::StringAtom& n)
{
    this->name = n;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
View::GetName() const
{
    return this->name;
}

//------------------------------------------------------------------------------
/**
*/
inline void
View::SetStage(const Ptr<Stage>& s)
{
    this->stage = s;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Stage>&
View::GetStage() const
{
    return this->stage;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CameraEntity>&
View::GetCameraEntity() const
{
    return this->camera;
}

//------------------------------------------------------------------------------
/**
*/
inline void
View::SetRenderTarget(const Ptr<CoreGraphics::RenderTarget>& rt)
{
    this->renderTarget = rt;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::RenderTarget>&
View::GetRenderTarget() const
{
    return this->renderTarget;
}

//------------------------------------------------------------------------------
/**
*/
inline void
View::AddDependency(const Ptr<View>& depView)
{
    this->dependencies.Append(depView);
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<View> >&
View::GetDependencies() const
{
    return this->dependencies;
}

//------------------------------------------------------------------------------
/**
*/
inline void
View::SetFrameShader(const Ptr<Frame::FrameShader>& shd)
{
    this->frameShader = shd;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Frame::FrameShader>&
View::GetFrameShader() const
{
    return this->frameShader;
}

} // namespace Graphics
//------------------------------------------------------------------------------
#endif
