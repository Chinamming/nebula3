#pragma once
#ifndef COREGRAPHICS_SHAPERENDERER_H
#define COREGRAPHICS_SHAPERENDERER_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::ShapeRenderer
    
    Render shapes for debug visualizations.
    
    (C) 2007 Radon Labs GmbH
*/
#if __WIN32__
#include "coregraphics/d3d9/d3d9shaperenderer.h"
namespace CoreGraphics
{
class ShapeRenderer : public Direct3D9::D3D9ShapeRenderer
{
    DeclareClass(ShapeRenderer);
    DeclareSingleton(ShapeRenderer);
public:
    /// constructor
    ShapeRenderer();
    /// destructor
    virtual ~ShapeRenderer();        
};
} // namespace CoreGraphics
#elif __XBOX360__
#include "coregraphics/xbox360/xbox360shaperenderer.h"
namespace CoreGraphics
{
class ShapeRenderer : public Xbox360::Xbox360ShapeRenderer
{
    DeclareClass(ShapeRenderer);
    DeclareSingleton(ShapeRenderer);
public:
    /// constructor
    ShapeRenderer();
    /// destructor
    virtual ~ShapeRenderer();
};
} // namespace CoreGraphics
#elif __WII__
#include "coregraphics/wii/wiishaperenderer.h"
namespace CoreGraphics
{
class ShapeRenderer : public Wii::WiiShapeRenderer
{
    DeclareClass(ShapeRenderer);
    DeclareSingleton(ShapeRenderer);
public:
    /// constructor
    ShapeRenderer();
    /// destructor
    virtual ~ShapeRenderer();
};
} // namespace CoreGraphics
#else
#error "ShapeRenderer class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
