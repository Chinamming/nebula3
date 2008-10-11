#pragma once
#ifndef COREGRAPHICS_PRIMITIVETOPOLOGY_H
#define COREGRAPHICS_PRIMITIVETOPOLOGY_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::PrimitiveTopology
    
    The primitive topology for a draw call.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/types.h"
#include "util/string.h"

//------------------------------------------------------------------------------
namespace CoreGraphics
{
class PrimitiveTopology
{
public:
    /// enumeration
    enum Code
    {
        InvalidPrimitiveTopology,

        PointList,
        LineList,
        LineStrip,
        TriangleList,
        TriangleStrip,
        LineListWithAdjacency,
        LineStripWithAdjacency,
        TriangleListWithAdjacency,
        TriangleStripWithAdjacency,
    };

    /// convert from string
    static Code FromString(const Util::String& str);
    /// convert to string
    static Util::String ToString(Code code);
    /// compute number of vertices/indices given a primitive topology and number of primitives
    static SizeT NumberOfVertices(Code topology, SizeT numPrimitives);
    /// compute number of primitives given a primitive type and number of vertices/indices
    static SizeT NumberOfPrimitives(Code topology, SizeT numVertices);

};

}; // namespace CoreGraphics
//------------------------------------------------------------------------------
#endif
