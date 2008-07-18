//------------------------------------------------------------------------------
//  modelnodetype.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/modelnodetype.h"

namespace Models
{

using namespace Util;

//------------------------------------------------------------------------------
/**
*/
ModelNodeType::Code
ModelNodeType::FromString(const String& str)
{
    if (str == "Solid") return Solid;
    else if (str == "Alpha") return Alpha;
    else if (str == "Occluder") return Occluder;
    else if (str == "ShadowCaster") return ShadowCaster;
    else
    {
        n_error("ModelNodeType::FromString(): invalid type string '%s'!", str.AsCharPtr());
        return InvalidModelNodeType;
    }
}

//------------------------------------------------------------------------------
/**
*/
String
ModelNodeType::ToString(Code c)
{
    switch (c)
    {
        case Solid:         return "Solid";
        case Alpha:         return "Alpha";
        case Occluder:      return "Occluder";
        case ShadowCaster:  return "ShadowCaster";
        default:
            n_error("ModelNodeType::ToString(): invalid type!");
            return "";
    }
}

} // namespace Models
