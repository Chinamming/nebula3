//------------------------------------------------------------------------------
//  primitivetopology.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/primitivetopology.h"

namespace CoreGraphics
{
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
PrimitiveTopology::Code
PrimitiveTopology::FromString(const String& str)
{
    if (str == "PointList")                       return PointList;
    else if (str == "LineList")                   return LineList;
    else if (str == "LineStrip")                  return LineStrip;
    else if (str == "TriangleList")               return TriangleList;
    else if (str == "TriangleStrip")              return TriangleStrip;
    else if (str == "LineListWithAdjacency")      return LineListWithAdjacency;
    else if (str == "LineStripWithAdjacency")     return LineStripWithAdjacency;
    else if (str == "TriangleListWithAdjacency")  return TriangleListWithAdjacency;
    else if (str == "TriangleStripWithAdjacency") return TriangleStripWithAdjacency;
    else
    {
        n_error("PrimitiveTopology::FromString(): invalid string '%s'!", str.AsCharPtr());
        return InvalidPrimitiveTopology;
    }
}

//------------------------------------------------------------------------------
/**
*/
String
PrimitiveTopology::ToString(Code code)
{
    switch (code)
    {
        case PointList:                  return "PointList";
        case LineList:                   return "LineList";
        case LineStrip:                  return "LineStrip";
        case TriangleList:               return "TriangleList";
        case TriangleStrip:              return "TriangleStrip";
        case LineListWithAdjacency:      return "LineListWithAdjacency";
        case LineStripWithAdjacency:     return "LineStripWithAdjacency";
        case TriangleListWithAdjacency:  return "TriangleListWithAdjacency";
        case TriangleStripWithAdjacency: return "TriangleStripWithAdjacency";
        default:
            n_error("PrimitiveTopology::ToString(): invalid topology code!");
            return "";
    }
}

//------------------------------------------------------------------------------
/**
    Computes the number of required vertices for a given primitive
    topology and number of primitives.
*/
SizeT
PrimitiveTopology::NumberOfVertices(Code topology, SizeT numPrimitives)
{
    switch (topology)
    {
        case PointList: 
            return numPrimitives;

        case LineList:
        case LineListWithAdjacency:
            return numPrimitives * 2;

        case LineStrip:
        case LineStripWithAdjacency:
            return numPrimitives + 1;

        case TriangleList:
        case TriangleListWithAdjacency:
            return numPrimitives * 3;

        case TriangleStrip:
        case TriangleStripWithAdjacency:
            return numPrimitives + 2;

        default:
            n_error("PrimitiveTopology::NumberOfVertices(): invalid topology!");
            return InvalidIndex;
    }
}

//------------------------------------------------------------------------------
/**
    Computes the number of primitives from a given primitive topology
    and number of vertices (the opposite of ComputeNumberOfVertices()).
*/
SizeT
PrimitiveTopology::NumberOfPrimitives(Code topology, SizeT numVertices)
{
    switch (topology)
    {
        case PointList:
            return numVertices;

        case LineList:
        case LineListWithAdjacency:
            return numVertices / 2;

        case LineStrip:
        case LineStripWithAdjacency:
            return numVertices - 1;

        case TriangleList:
        case TriangleListWithAdjacency:
            return numVertices / 3;

        case TriangleStrip:
        case TriangleStripWithAdjacency:
            return numVertices - 2;

        default:
            n_error("PrimitiveTopology::NumberOfPrimitives(): invalid topology!");
            return InvalidIndex;
    }
}

} // namespace CoreGraphics
