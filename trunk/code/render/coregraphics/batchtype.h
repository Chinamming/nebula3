#pragma once
#ifndef COREGRAPHICS_BATCHTYPE_H
#define COREGRAPHICS_BATCHTYPE_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::BatchType
    
    Batch type hints for the render device and render targets. Indicates
    the type of objects that are rendered in a batch.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/types.h"
#include "util/string.h"

//------------------------------------------------------------------------------
namespace CoreGraphics
{
class BatchType
{
public:
    /// batch type enum
    enum Code
    {
        Depth,
        Solid,
        Alpha,
        SolidAutoZ,     // Xbox360 specific
        Shapes,         // ShapeRenderer shapes
        Text,           // TextRenderer elements
    
        InvalidBatchType
    };
    
    /// convert from string
    static BatchType::Code FromString(const Util::String& str);
    /// convert to string
    static Util::String ToString(BatchType::Code c);
};

//------------------------------------------------------------------------------
/**
*/
inline BatchType::Code
BatchType::FromString(const Util::String& str)
{
    if (str == "Depth")            return Depth;
    else if (str == "Solid")       return Solid;
    else if (str == "Alpha")       return Alpha;
    else if (str == "SolidAutoZ")  return SolidAutoZ;
    else if (str == "Shapes")      return Shapes;
    else if (str == "Text")        return Text;
    else
    {
        n_error("BatchType::FromString(): invalid string '%s'!", str.AsCharPtr());
        return InvalidBatchType;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline Util::String
BatchType::ToString(BatchType::Code c)
{
    switch (c)
    {
        case Depth:         return "Depth";
        case Solid:         return "Solid";
        case Alpha:         return "Alpha";
        case SolidAutoZ:    return "SolidAutoZ";
        case Shapes:        return "Shapes";
        case Text:          return "Text";
        default:
            n_error("BatchType::ToString(): invalid batch type!");
            return "";
    }
}

} // namespace CoreGraphics
//------------------------------------------------------------------------------
#endif
    