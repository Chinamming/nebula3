//------------------------------------------------------------------------------
//  abstractlightentity.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/abstractlightentity.h"

namespace Graphics
{
ImplementClass(Graphics::AbstractLightEntity, 'GALE', Graphics::GraphicsEntity);

using namespace Lighting;

//------------------------------------------------------------------------------
/**
*/
AbstractLightEntity::AbstractLightEntity() :
    lightType(LightType::InvalidLightType),
    color(1.0f, 1.0f, 1.0f, 1.0f),
    projMapUvOffsetAndScale(0.5f, 0.5f, 0.5f, -0.5f),
    castShadows(false)
{
    this->SetType(GraphicsEntityType::Light);
}

//------------------------------------------------------------------------------
/**
*/
AbstractLightEntity::~AbstractLightEntity()
{
    // empty
}

} // namespace AsyncGraphics