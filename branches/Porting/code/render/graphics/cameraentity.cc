//------------------------------------------------------------------------------
//  cameraentity.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/cameraentity.h"
#include "coregraphics/displaydevice.h"
#include "graphics/view.h"

namespace Graphics
{
ImplementClass(Graphics::CameraEntity, 'CMRE', Graphics::GraphicsEntity);

using namespace Math;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
CameraEntity::CameraEntity() :
    viewProjDirty(true)
{
    this->SetType(CameraType);
    float aspectRatio = DisplayDevice::Instance()->GetDisplayMode().GetAspectRatio();
    this->SetupPerspectiveFov(n_deg2rad(60.0f), aspectRatio, 0.01f, 1000.0f);
    this->viewMatrix = matrix44::identity();
    this->viewProjMatrix = matrix44::identity();
}

//------------------------------------------------------------------------------
/**
*/
CameraEntity::~CameraEntity()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
CameraEntity::IsAttachedToView() const
{
    return this->view.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
void
CameraEntity::OnAttachToView(const Ptr<View>& v)
{
    n_assert(!this->view.isvalid());
    this->view = v;
}

//------------------------------------------------------------------------------
/**
*/
void
CameraEntity::OnRemoveFromView(const Ptr<View>& v)
{
    n_assert(this->view.isvalid() && this->view == v);
    this->view = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
CameraEntity::OnDeactivate()
{
    // if we're currently attached to a view, we need to detach first
    if (this->IsAttachedToView())
    {
        this->view->SetCameraEntity(0);
    }
    GraphicsEntity::OnDeactivate();
}

//------------------------------------------------------------------------------
/**
    Setup camera as perspective projection.
*/
void
CameraEntity::SetupPerspectiveFov(float fov_, float aspect_, float zNear_, float zFar_)
{
    this->isPersp = true;
    this->zNear   = zNear_;
    this->zFar    = zFar_;
    this->fov     = fov_;
    this->aspect  = aspect_;

    this->projMatrix = matrix44::perspfovrh(this->fov, this->aspect, this->zNear, this->zFar);

    this->nearWidth  = 2.0f * this->zNear / this->projMatrix.getrow0().x();
    this->nearHeight = 2.0f * this->zNear / this->projMatrix.getrow1().y();
    this->farWidth   = (this->nearWidth / this->zNear) * this->zFar;
    this->farHeight  = (this->nearHeight / this->zNear) * this->zFar;

    this->viewProjDirty = true;
}

//------------------------------------------------------------------------------
/**
    Setup camera as orthogonal projection.
*/
void
CameraEntity::SetupOrthogonal(float w, float h, float zNear_, float zFar_)
{
    this->isPersp    = false;
    this->zNear      = zNear_;
    this->zFar       = zFar_;
    this->fov        = 0.0f;
    this->aspect     = w / h;
    this->nearWidth  = w;
    this->nearHeight = h;
    this->farWidth   = w;
    this->farHeight  = h;

    this->projMatrix = matrix44::orthorh(w, h, this->zNear, this->zFar);

    this->viewProjDirty = true;
}

//------------------------------------------------------------------------------
/**
    We need to keep track of modifications of the transformation matrix.
*/
void
CameraEntity::OnTransformChanged()
{
    this->viewMatrix = matrix44::inverse(this->GetTransform());
    this->viewProjDirty = true;
}

//------------------------------------------------------------------------------
/**
    Computes the clip status of a bounding box in global space against
    the view volume of this camera entity.
*/    
ClipStatus::Type
CameraEntity::ComputeClipStatus(const bbox& box)
{
    ClipStatus::Type clipStatus = box.clipstatus(this->GetViewProjTransform());
    return clipStatus;
}

//------------------------------------------------------------------------------
/**
    Updates the view-projection matrix.
*/
void
CameraEntity::UpdateViewProjMatrix()
{
    n_assert(this->viewProjDirty);
    this->viewProjDirty = false;
    this->viewProjMatrix = matrix44::multiply(this->viewMatrix, this->projMatrix);
}

} // namespace Graphics
