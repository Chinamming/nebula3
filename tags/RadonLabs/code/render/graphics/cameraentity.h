#pragma once
#ifndef GRAPHICS_CAMERAENTITY_H
#define GRAPHICS_CAMERAENTITY_H
//------------------------------------------------------------------------------
/**
    @class Graphics::CameraEntity
  
    Represents a camera attached to a graphics stage. Any number of
    cameras can be attached to a stage.
    
    (C) 2007 Radon Labs GmbH
*/    
#include "graphics/graphicsentity.h"

//------------------------------------------------------------------------------
namespace Graphics
{
class CameraEntity : public GraphicsEntity
{
    DeclareClass(CameraEntity);
public:
    /// constructor
    CameraEntity();
    /// destructor
    virtual ~CameraEntity();
    
    /// return true if camera is attached to a View
    bool IsAttachedToView() const;
    /// compute clip status against bounding box
    virtual Math::ClipStatus::Type ComputeClipStatus(const Math::bbox& box);

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
    friend class View;

    /// called before entity is destroyed
    virtual void OnDeactivate();
    /// called by View when camera is attached to that view
    void OnAttachToView(const Ptr<View>& view);
    /// called by View when camera becomes detached from view
    void OnRemoveFromView(const Ptr<View>& view);
    /// called when transform matrix changed
    virtual void OnTransformChanged();
    /// update the view projection matrix
    void UpdateViewProjMatrix();

    Ptr<View> view;

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
inline const Math::matrix44&
CameraEntity::GetProjTransform() const
{
    return this->projMatrix;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
CameraEntity::GetViewTransform() const
{
    return this->viewMatrix;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
CameraEntity::GetViewProjTransform()
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
CameraEntity::IsPerspective() const
{
    return this->isPersp;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
CameraEntity::IsOrthogonal() const
{
    return !this->isPersp;
}

//------------------------------------------------------------------------------
/**
*/
inline float
CameraEntity::GetZNear() const
{
    return this->zNear;
}

//------------------------------------------------------------------------------
/**
*/
inline float
CameraEntity::GetZFar() const
{
    return this->zFar;
}

//------------------------------------------------------------------------------
/**
*/
inline float
CameraEntity::GetFov() const
{
    return this->fov;
}

//------------------------------------------------------------------------------
/**
*/
inline float
CameraEntity::GetAspect() const
{
    return this->aspect;
}

//------------------------------------------------------------------------------
/**
*/
inline float
CameraEntity::GetNearWidth() const
{
    return this->nearWidth;
}

//------------------------------------------------------------------------------
/**
*/
inline float
CameraEntity::GetNearHeight() const
{
    return this->nearHeight;
}

//------------------------------------------------------------------------------
/**
*/
inline float
CameraEntity::GetFarWidth() const
{
    return this->farWidth;
}

//------------------------------------------------------------------------------
/**
*/
inline float
CameraEntity::GetFarHeight() const
{
    return this->farHeight;
}

} // namespace Graphics
//------------------------------------------------------------------------------
#endif
