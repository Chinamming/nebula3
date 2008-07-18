//------------------------------------------------------------------------------
//  lightentity.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "lighting/lightentity.h"
#include "resources/resourcemanager.h"

namespace Lighting
{
ImplementClass(Lighting::LightEntity, 'LGTE', Graphics::GraphicsEntity);

using namespace CoreGraphics;
using namespace Graphics;
using namespace Math;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
LightEntity::LightEntity() :
    invTransform(matrix44::identity()),
    color(1.0f, 1.0f, 1.0f, 1.0f),
    projMapUvOffset(0.5f, 0.5f, 0.5f, -0.5f),
    shaderLightType(ShaderLightType::Point),
    castShadows(false),
    shadowBufferUvOffset(0.5f, 0.5f, 0.5f, -0.5f)
{
    this->SetType(LightType);
}

//------------------------------------------------------------------------------
/**
*/
LightEntity::~LightEntity()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ClipStatus::Type
LightEntity::ComputeClipStatus(const bbox& box)
{
    // FIXME: compute more accurate result based on light source type!
    return this->GetGlobalBoundingBox().clipstatus(box);
}

//------------------------------------------------------------------------------
/**
*/
void
LightEntity::OnTransformChanged()
{
    GraphicsEntity::OnTransformChanged();

    // update our inverse transform
    this->invTransform = matrix44::inverse(this->transform);

    // compute light projection matrix from bounding box size at far z
    float widthAtFarZ  = this->transform.row0().length() * 2.0f;
    float heightAtFarZ = this->transform.row1().length() * 2.0f;
    float nearZ = 0.1f; // put the near plane at 10cm
    float farZ = this->transform.row2().length();    
    n_assert(farZ > 0.0f);
    if (nearZ >= farZ)
    {
        nearZ = farZ / 2.0f;
    }
    float widthAtNearZ  = (widthAtFarZ / farZ) * nearZ;
    float heightAtNearZ = (heightAtFarZ / farZ) * nearZ;
    this->projTransform = matrix44::persprh(widthAtNearZ, heightAtNearZ, nearZ, farZ);

    // compute the invLightProj matrix which transforms from world space to light space
    this->invLightProjTransform = matrix44::multiply(this->invTransform, this->projTransform);
}

} // namespace Graphics
