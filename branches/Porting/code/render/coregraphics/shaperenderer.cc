//------------------------------------------------------------------------------
//  shaperenderer.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/shaperenderer.h"

namespace CoreGraphics
{
#if __WIN32__
ImplementClass(CoreGraphics::ShapeRenderer, 'SHPR', Direct3D9::D3D9ShapeRenderer);
#elif __XBOX360__
ImplementClass(CoreGraphics::ShapeRenderer, 'SHPR', Xbox360::Xbox360ShapeRenderer);
#elif __WII__
ImplementClass(CoreGraphics::ShapeRenderer, 'SHPR', Wii::WiiShapeRenderer);
#else
#error "ShapeRenderer class not implemented on this platform!"
#endif
ImplementSingleton(CoreGraphics::ShapeRenderer);

//------------------------------------------------------------------------------
/**
*/
ShapeRenderer::ShapeRenderer()
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
ShapeRenderer::~ShapeRenderer()
{
    DestructSingleton;
}

} // namespace CoreGraphics
    
