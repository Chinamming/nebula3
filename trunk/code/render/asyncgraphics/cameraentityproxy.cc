//------------------------------------------------------------------------------
//  cameraentityproxy.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "asyncgraphics/cameraentityproxy.h"
#include "asyncgraphics/displayproxy.h"
#include "asyncgraphics/stageproxy.h"

namespace AsyncGraphics
{
ImplementClass(AsyncGraphics::CameraEntityProxy, 'ACEP', AsyncGraphics::GraphicsEntityProxy);

using namespace Util;
using namespace Math;
using namespace Graphics;

//------------------------------------------------------------------------------
/**
*/
CameraEntityProxy::CameraEntityProxy() :
    viewProjDirty(true)
{
    this->SetType(GraphicsEntity::CameraType);
    float aspectRatio = DisplayProxy::Instance()->GetDisplayMode().GetAspectRatio();
    this->SetupPerspectiveFov(n_deg2rad(60.0f), aspectRatio, 0.01f, 1000.0f);
    this->viewMatrix = matrix44::identity();
    this->viewProjMatrix = matrix44::identity();
}

//------------------------------------------------------------------------------
/**
*/
CameraEntityProxy::~CameraEntityProxy()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Setup the server-side camera entity.
*/
void
CameraEntityProxy::Setup(const Ptr<StageProxy>& stageProxy_)
{
    n_assert(this->viewName.Value().IsValid());

    GraphicsEntityProxy::Setup(stageProxy_);

    // send off an entity creation message
    Ptr<CreateCameraEntity> msg = CreateCameraEntity::Create();
    msg->SetStageName(this->stageProxy->GetName());
    msg->SetTransform(this->transform);
    msg->SetVisible(this->isVisible);
    msg->SetViewName(this->viewName);
    msg->SetIsPerspective(this->isPersp);
    msg->SetNearPlane(this->zNear);
    msg->SetFarPlane(this->zFar);
    if (this->isPersp)
    {
        msg->SetPerspFieldOfView(this->fov);
        msg->SetPerspAspectRatio(this->aspect);
    }
    else
    {
        msg->SetOrthoWidth(this->nearWidth);
        msg->SetOrthoHeight(this->nearHeight);
    }
    this->SendCreateMsg(msg.cast<CreateGraphicsEntity>());
}

//------------------------------------------------------------------------------
/**
    Setup camera as perspective projection.
    NOTE: this method is a copy of CameraEntity::SetupPerspectiveFov()!
*/
void
CameraEntityProxy::SetupPerspectiveFov(float fov_, float aspect_, float zNear_, float zFar_)
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
    NOTE: this method is a copy of CameraEntity::SetupOrthogonal()!
*/
void
CameraEntityProxy::SetupOrthogonal(float w, float h, float zNear_, float zFar_)
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
CameraEntityProxy::OnTransformChanged()
{
    this->viewMatrix = matrix44::inverse(this->GetTransform());
    this->viewProjDirty = true;
}

//------------------------------------------------------------------------------
/**
    Updates the view-projection matrix.
*/
void
CameraEntityProxy::UpdateViewProjMatrix()
{
    n_assert(this->viewProjDirty);
    this->viewProjDirty = false;
    this->viewProjMatrix = matrix44::multiply(this->viewMatrix, this->projMatrix);
}

} // namespace AsyncGraphics
