#pragma once
#ifndef ASYNCGRAPHICS_CAMERAENTITYPROXY_H
#define ASYNCGRAPHICS_CAMERAENTITYPROXY_H
//------------------------------------------------------------------------------
/**
    @class AsyncGraphics::CameraEntityProxy
    
    Client-side proxy of a Graphics::CameraEntity. NOTE: all getter-methods
    of this class return client-side cached values, not the actual server-side
    values. Thus they may be off by some amount, since the render thread
    may run at a different frame rate then the client thread!
    
    (C) 2007 Radon Labs GmbH
*/
#include "asyncgraphics/graphicsentityproxy.h"

//------------------------------------------------------------------------------
namespace AsyncGraphics
{
class CameraEntityProxy : public GraphicsEntityProxy
{
    DeclareClass(CameraEntityProxy);
public:
    /// constructor
    CameraEntityProxy();
    /// destructor
    virtual ~CameraEntityProxy();

    /// set the view name which should use this camera
    void SetViewName(const Util::StringAtom& n);
    /// get the view name which should use this camera
    const Util::StringAtom& GetViewName() const;
    /// setup camera for perspective field-of-view projection transform
    void SetupPerspectiveFov(float fov, float aspect, float zNear, float zFar);
    /// setup camera for orthogonal projection transform
    void SetupOrthogonal(float w, float h, float zNear, float zFar);

    /// get projection matrix
    const Math::matrix44& GetProjTransform() const;
    /// get view transform (inverse transform)
    const Math::matrix44& GetViewTransform() const;
    /// get view projection matrix (non-const!)
    const Math::matrix44& GetViewProjTransform();

    /// return true if this is a perspective projection
    bool IsPerspective() const;
    /// return true if this is an orthogonal transform
    bool IsOrthogonal() const;
    /// get near plane distance
    float GetZNear() const;
    /// get far plane distance
    float GetZFar() const;
    /// get field-of-view (only if perspective)
    float GetFov() const;
    /// get aspect ration (only if perspective)
    float GetAspect() const;
    /// get width of near plane
    float GetNearWidth() const;
    /// get height of near plane
    float GetNearHeight() const;
    /// get width of far plane
    float GetFarWidth() const;
    /// get height of far plane
    float GetFarHeight() const;

protected:
    /// called by stage when entity should setup itself
    virtual void Setup(const Ptr<StageProxy>& stage);
    /// called when transform matrix changed
    virtual void OnTransformChanged();
    /// update the view projection matrix
    void UpdateViewProjMatrix();

    Util::StringAtom viewName;
    Math::matrix44 projMatrix;
    Math::matrix44 viewMatrix;
    Math::matrix44 viewProjMatrix;
    bool viewProjDirty;

    bool isPersp;
    float zNear;
    float zFar;
    float fov;
    float aspect;
    float nearWidth;
    float nearHeight;
    float farWidth;
    float farHeight;
};

//------------------------------------------------------------------------------
/**
*/
inline void
CameraEntityProxy::SetViewName(const Util::StringAtom& n)
{
    this->viewName = n;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
CameraEntityProxy::GetViewName() const
{
    return this->viewName;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
CameraEntityProxy::GetProjTransform() const
{
    return this->projMatrix;
}

//------------------------------------------------------------------------------
/**
    NOTE: The matrix returned here may be off a little bit from view
    matrix used in the render thread because the game thread may run
    at a slower frame rate then rendering!
    It is not possible to get an exact view matrix outside of the render
    thread.
*/
inline const Math::matrix44&
CameraEntityProxy::GetViewTransform() const
{
    return this->viewMatrix;
}

//------------------------------------------------------------------------------
/**
    NOTE: The matrix returned here may be off a little bit from viewProj
    matrix used in the render thread because the game thread may run
    at a slower frame rate then rendering!
    It is not possible to get an exact viewProj matrix outside of the render
    thread.
*/
inline const Math::matrix44&
CameraEntityProxy::GetViewProjTransform()
{
    if (this->viewProjDirty)
    {
        this->UpdateViewProjMatrix();
    }
    return this->viewProjMatrix;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
CameraEntityProxy::IsPerspective() const
{
    return this->isPersp;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
CameraEntityProxy::IsOrthogonal() const
{
    return !this->isPersp;
}

//------------------------------------------------------------------------------
/**
*/
inline float
CameraEntityProxy::GetZNear() const
{
    return this->zNear;
}

//------------------------------------------------------------------------------
/**
*/
inline float
CameraEntityProxy::GetZFar() const
{
    return this->zFar;
}

//------------------------------------------------------------------------------
/**
*/
inline float
CameraEntityProxy::GetFov() const
{
    return this->fov;
}

//------------------------------------------------------------------------------
/**
*/
inline float
CameraEntityProxy::GetAspect() const
{
    return this->aspect;
}

//------------------------------------------------------------------------------
/**
*/
inline float
CameraEntityProxy::GetNearWidth() const
{
    return this->nearWidth;
}

//------------------------------------------------------------------------------
/**
*/
inline float
CameraEntityProxy::GetNearHeight() const
{
    return this->nearHeight;
}

//------------------------------------------------------------------------------
/**
*/
inline float
CameraEntityProxy::GetFarWidth() const
{
    return this->farWidth;
}

//------------------------------------------------------------------------------
/**
*/
inline float
CameraEntityProxy::GetFarHeight() const
{
    return this->farHeight;
}

} // namespace AsyncGraphics
//------------------------------------------------------------------------------
#endif
    