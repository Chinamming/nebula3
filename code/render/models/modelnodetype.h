#pragma once
#ifndef MODELS_MODELNODETYPE_H
#define MODELS_MODELNODETYPE_H
//------------------------------------------------------------------------------
/**
    @class Models::ModelNodeType
  
    ModelNodeTypes identify a ModelNode for a specific rendering pass.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/types.h"
#include "util/string.h"

//------------------------------------------------------------------------------
namespace Models
{
class ModelNodeType
{
public:
    /// type enum
    enum Code
    {
        Solid = 0,              // a solid shape
        Alpha,                  // a translucent shape
        Occluder,               // an occluder
        ShadowCaster,           // a shadow caster

		Refractive,				// a refraction model type

        NumModelNodeTypes,
        InvalidModelNodeType,
    };
    
    /// convert from string
    static Code FromString(const Util::String& str);
    /// convert to string
    static Util::String ToString(Code c);
};

}
//------------------------------------------------------------------------------
#endif

