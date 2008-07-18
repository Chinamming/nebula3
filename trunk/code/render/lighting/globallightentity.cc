//------------------------------------------------------------------------------
//  globallightentity.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "lighting/globallightentity.h"
#include "graphics/stage.h"
#include "graphics/cell.h"

namespace Lighting
{
ImplementClass(Lighting::GlobalLightEntity, 'GLBE', Lighting::AbstractLightEntity);

using namespace Math;
using namespace Graphics;

//------------------------------------------------------------------------------
/**
*/
GlobalLightEntity::GlobalLightEntity() :
    backLightColor(0.0f, 0.0f, 0.0f, 0.0f),
    lightDir(0.0f, 0.0f, -1.0f)
{
    this->SetLightType(LightType::Global);
}

//------------------------------------------------------------------------------
/**
*/
ClipStatus::Type
GlobalLightEntity::ComputeClipStatus(const Math::bbox& box)
{
    // since we are essentially a directional light, everything is visible,
    // we depend on the visibility detection code in the Cell class to
    // only create light links for objects that are actually visible
    return ClipStatus::Inside;
}

//------------------------------------------------------------------------------
/**
*/
void
GlobalLightEntity::OnTransformChanged()
{
    AbstractLightEntity::OnTransformChanged();

    // extract the light's direction from the transformation matrix
    this->lightDir = float4::normalize(this->transform.getrow2());

    // extend transformation to a very big size, since we need to be visible
    // from everywhere in the stage
    const float size = 100000.0f;
    this->transform.setx_component(float4::normalize(this->transform.getx_component()) * size);
    this->transform.sety_component(float4::normalize(this->transform.gety_component()) * size);
    this->transform.setz_component(float4::normalize(this->transform.getz_component()) * size);
}

} // namespace Lighting
