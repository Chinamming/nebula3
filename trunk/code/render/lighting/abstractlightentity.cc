//------------------------------------------------------------------------------
//  abstractlightentity.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "lighting/abstractlightentity.h"

namespace Lighting
{
ImplementClass(Lighting::AbstractLightEntity, 'ALIE', Graphics::GraphicsEntity);

using namespace Math;

//------------------------------------------------------------------------------
/**
*/
AbstractLightEntity::AbstractLightEntity() :
    lightType(LightType::InvalidLightType),
    color(1.0f, 1.0f, 1.0f, 1.0f),
    projMapUvOffsetAndScale(0.5f, 0.5f, 0.5f, -0.5f),
    shadowBufferUvOffsetAndScale(0.5f, 0.5f, 0.5f, -0.5f),
    castShadows(false),
    invTransform(matrix44::identity()),
    projTransform(matrix44::identity()),
    invLightProjTransform(matrix44::identity())
{
    this->SetType(LightType);
}

//------------------------------------------------------------------------------
/**
*/
AbstractLightEntity::~AbstractLightEntity()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
AbstractLightEntity::OnTransformChanged()
{
    GraphicsEntity::OnTransformChanged();

    // update inverse transform
    this->invTransform = matrix44::inverse(this->transform);
}

} // namespace Lighting
