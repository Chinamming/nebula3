//------------------------------------------------------------------------------
//  physics/sliderjoint.cc
//  (C) 2003 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "physics/sliderjoint.h"
#include "physics/rigidbody.h"

namespace Physics
{
ImplementClass(Physics::SliderJoint, 'PSLJ', Physics::Joint);

//------------------------------------------------------------------------------
/**
*/
SliderJoint::SliderJoint() : 
    Joint(Joint::SliderJoint)
{
    this->axisParams.SetAxis(Math::vector(1.0f, 0.0f, 0.0f));
}

//------------------------------------------------------------------------------
/**
*/
SliderJoint::~SliderJoint()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    NOTE: it is important that rigid bodies are added 
    (happens in Joint::Attach()) before joint transforms are set!!!
*/
void
SliderJoint::Attach(dWorldID worldID, dJointGroupID groupID, const Math::matrix44& m)
{
    // create ODE joint
    this->odeJointId = dJointCreateSlider(worldID, groupID);

    // configure ODE joint
    if (this->axisParams.IsLoStopEnabled())
    {
        dJointSetSliderParam(this->odeJointId, dParamLoStop, this->axisParams.GetLoStop());
    }
    if (this->axisParams.IsHiStopEnabled())
    {
        dJointSetSliderParam(this->odeJointId, dParamHiStop, this->axisParams.GetHiStop());
    }
    dJointSetSliderParam(this->odeJointId, dParamVel, this->axisParams.GetVelocity());
    dJointSetSliderParam(this->odeJointId, dParamFMax, this->axisParams.GetFMax());
    dJointSetSliderParam(this->odeJointId, dParamFudgeFactor, this->axisParams.GetFudgeFactor());
    dJointSetSliderParam(this->odeJointId, dParamBounce, this->axisParams.GetBounce());
    dJointSetSliderParam(this->odeJointId, dParamCFM, this->axisParams.GetCFM());
    dJointSetSliderParam(this->odeJointId, dParamStopERP, this->axisParams.GetStopERP());
    dJointSetSliderParam(this->odeJointId, dParamStopCFM, this->axisParams.GetStopCFM());

    // hand to parent class
    Joint::Attach(worldID, groupID, m);

    // configure ODE joint
    this->UpdateTransform(m);
}

//------------------------------------------------------------------------------
/**
*/
void 
SliderJoint::UpdateTransform(const Math::matrix44& m)
{
    matrix44 m44(m.getx_component(), m.gety_component(), m.getz_component(), vector::nullvec());
    Math::vector a = vector::transform(this->axisParams.GetAxis(), m44);
    dJointSetSliderAxis(this->odeJointId, a.x(), a.y(), a.z());
}

} // namespace Physics